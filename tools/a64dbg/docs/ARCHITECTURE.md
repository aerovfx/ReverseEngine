# a64dbg — Kiến trúc

## Hai tầng backend

```
┌─────────────────────────────────────────────────────────────┐
│  GUI (Qt 6 Widgets, main thread)                             │
│  DisasmView · DumpView · StackView · RegisterView · ...      │
│  BreakpointManager · SymbolProvider · PluginHost · Script    │
└───────────────▲─────────────────────────────▲────────────────┘
                │ Qt queued events (thread-safe queue)          │
┌───────────────┴───────────────┐ ┌─────────────┴───────────────┐
│  IDebugBackend (interface)     │ │  ISymbolProvider (iface)    │
└───────────────┬───────────────┘ └─────────────┬───────────────┘
        ┌───────┴────────┐                ┌──────┴───────┐
        │ MachBackend    │                │ LLDBBackend  │
        │ (tự viết, core)│                │ (SB API)     │
        └───────┬────────┘                └──────────────┘
   ┌────────────┼─────────────────────────────┐
   │ task_for_pid · exception port · mach_vm  │
   │ thread_get/set_state · arm_debug_state64 │
   └────────────┴─────────────────────────────┘
   Capstone (disasm) · Keystone (assemble) · Unicorn (emulate, tùy chọn)
```

- **Mach core** sở hữu primitive: attach/detach (`task_for_pid`), bắt exception (Mach exception ports + vòng `mach_msg`), đọc/ghi memory (`mach_vm_read_overwrite/write`), đọc/ghi register (`ARM_THREAD_STATE64`), breakpoint phần cứng/watchpoint (`ARM_DEBUG_STATE64`: bvr/bcr/wvr/wcr), single-step (MDSCR_EL1.SS).
- **LLDB layer** đảm nhận thứ khó làm đúng từ đầu: symbolication (dyld shared cache, dSYM/DWARF), demangle, source line, step over/out, `expr`.
- Mach core là **nguồn sự thật** về trạng thái dừng; LLDB đọc/step trên cùng pid đã đồng bộ.

## Phân chia thư mục (ownership map)

| Thư mục | Target | Sở hữu | Trạng thái |
|---|---|---|---|
| `src/common` | `a64dbg_common` | Log, Version | Phase 0 ✓ |
| `src/model` | `a64dbg_model` | Session, BreakpointManager (sau) | Phase 0 stub |
| `src/disasm` | `a64dbg_disasm` | Capstone wrapper (ARM64) + instruction cache | Phase 2 ✓ (disasm ARM64) |
| `src/macho` | `a64dbg_macho` | Mach-O parser (segment/symbol/entry) | Phase 2 ✓ |
| `src/symbol` | `a64dbg_symbol` | SymbolProvider (TASK_DYLD_INFO + LC_SYMTAB) | Phase 4 ✓ |
| `src/core` | `a64dbg_core` | IDebugBackend + MachBackend | Phase 3 ✓ (attach/mem/reg/BRK bp/hw bp/watchpoint/SS/dance) |
| `src/lldb` | `a64dbg_lldb` | LLDBBackend + ISymbolProvider | defer (SB API headers không có trong Xcode) |
| `src/app` | `a64dbg` | main + MainWindow + theme | Phase 2 ✓ (wire controller) |
| `src/ui` | `a64dbg_ui` | DebuggerController + DisasmView/DumpView | Phase 5 ✓ (step over/out + stack walk/search/patch) |
| `src/plugin` | `a64dbg_plugin` | PluginHost (dlopen + C ABI `dbg_sdk.h`) | Phase 6 ✓ |
| `src/script` | `a64dbg_script` | ScriptEngine (QJSEngine + `debug` object) | Phase 6 ✓ |

## Cấu trúc cây

```
a64dbg/
  CMakeLists.txt · CMakePresets.json
  cmake/            # CompilerFlags.cmake, FetchCapstone.cmake, ...
  src/{common,model,disasm,core,lldb,app,...}/
  include/dbg_sdk.h # header plugin (Phase 6)
  entitlements/     # debugger.entitlements
  tests/            # CTest
  docs/             # ARCHITECTURE.md, features.md, PLUGINS.md
  plugins/ · scripts/  (Phase 6)
```

## Ghi chú dependency

- **Capstone** (5.0.9, ghim qua FetchContent, build diet ARM64+X86): Phase 0 đã nối pipeline (xem `src/disasm/Disassembler.cpp`).
- **Keystone**: Phase 5 (assemble/patch). Kéo khi tới.
- **LLDB SB API**: đã xác nhận Phase 4 — Xcode `LLDB.framework` chỉ có binary driver 188 MB + plugin, KHÔNG có SB API headers (`SBProcess.h` không tồn tại trong Xcode). → thay bằng **symbolication native** (`src/symbol`: `TASK_DYLD_INFO` + parse `LC_SYMTAB`) và step over/out bằng disasm + temp bp. Dùng LLDB SB API chỉ khi vendor llvm-project headers + build lldb (bỏ qua ở bản này).
- **QtQml (scripting)**: brew `qtdeclarative` build cho macOS 26.0 → `a64dbg_script` (QJSEngine) chỉ là thư viện standalone + test, KHÔNG link vào app `.app` để giữ deployment target 14.0. App vẫn dùng Qt6::Widgets thuần.
- **Lua** (scripting): Phase 6.

## Ràng buộc nền tảng

- App phải code-sign với entitlement `com.apple.security.cs.debugger` (hoặc chạy root) để `task_for_pid` lên tiến trình khác.
- SIP chặn attach tiến trình Apple ký hệ thống.
- Demo an toàn: launch child bằng `posix_spawn(POSIX_SPAWN_START_SUSPENDED)` — không cần entitlement để gỡ lỗi tiến trình do mình tạo.

## Gotcha nền tảng ARM64 (đã đo trên máy thật — Phase 1)

- `task_for_pid`: ký ad-hoc + entitlement debugger **được** taskgated chấp nhận cho con của mình (kr=0); launchd/SIP vẫn kr=5.
- **BRK mềm**: exception `EXC_BREAKPOINT` có `code[0]=EXC_ARM_BREAKPOINT(1)`, `code[1]=địa chỉ lệnh BRK`, và `PC` trỏ **TẠI** lệnh BRK (khác x86 trỏ +1 sau `int3`). → resume chỉ cần khôi phục lệnh gốc, PC đã đúng, không cần lùi.
- **Single-step (MDSCR_EL1.SS)**: cũng gửi `EXC_BREAKPOINT code[0]=1` nhưng `code[1]=0`; `PC` = lệnh kế tiếp. Phân biệt bằng `code[1]`.
- **W^X**: cấm một trang vừa write vừa execute. Patch code = `mach_vm_protect(READ|WRITE|COPY)` → `mach_vm_write` → khôi phục `READ|EXECUTE` (target phải đang suspend vì khoảng mất-execute).
- Tên field (arm64, không phải arm64e): `arm_thread_state64_t` dùng `__x/__fp/__lr/__sp/__pc/__cpsr`; `arm_debug_state64_t` dùng `__bvr/__bcr/__wvr/__wcr/__mdscr_el1` (có `__` vì biến thể `__DARWIN_UNIX03`).
- Không có `EXC_MASK_SINGLE_STEP` trên macOS — single-step nằm trong `EXC_MASK_BREAKPOINT`.
- `mach_exc_server` không có trong libSystem → vendor MIG-generated (`src/core/mig/`) từ `mach_exc.defs`.
- **Hardware breakpoint (bvr)**: gửi `EXC_BREAKPOINT code[0]=1` (giống BRK), `code[1]=địa chỉ` — phân biệt bằng tra bảng `m_hwBps`.
- **Watchpoint (wvr)**: gửi `EXC_BREAKPOINT code[0]=EXC_ARM_DA_DEBUG(0x102)`, `code[1]=địa chỉ được watch` (KHÔNG phải EXC_BAD_ACCESS).
- **Breakpoint dance** (continue giữ bp): khôi phục lệnh gốc → single-step 1 lệnh → re-patch BRK → continue; single-step trung gian phải "transparent" (không dừng cho user).
- **Detach phải khôi phục breakpoint trước**: nếu detach khi đang dừng ở BRK mà không restore lệnh gốc, reply sẽ resume target NGAY TẠI lệnh BRK → re-trigger → kẹt trong exception (SIGKILL không kết liễu kịp). Fix: detach() restore mọi software bp + tắt hw bp/watchpoint trước khi resume.
