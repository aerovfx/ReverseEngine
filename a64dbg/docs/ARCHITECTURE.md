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
| `src/core` | `a64dbg_core` | IDebugBackend + MachBackend | Phase 1 ✓ (attach/mem/reg/BRK bp/exception/single-step) |
| `src/lldb` | `a64dbg_lldb` | LLDBBackend + ISymbolProvider | Phase 4 |
| `src/app` | `a64dbg` | main + MainWindow + theme | Phase 2 ✓ (wire controller) |
| `src/ui` | `a64dbg_ui` | DebuggerController + DisasmView/DumpView | Phase 2 ✓ |
| `src/plugin` | `a64dbg_plugin` | PluginHost + scripting | Phase 6 |

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
- **LLDB SB API**: defer Phase 4 — Xcode cung cấp `lldb` binary và `LLDB.framework` nhưng KHÔNG lộ SB API headers (`lldb/API/*.h`) trong SDK; cần lấy headers từ llvm-project (khớp phiên bản) hoặc build lldb, sẽ khảo sát ở Phase 4.
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
