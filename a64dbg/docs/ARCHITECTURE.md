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
| `src/disasm` | `a64dbg_disasm` | Capstone wrapper + instruction cache | Phase 0 stub (version), Phase 2 full |
| `src/core` | `a64dbg_core` | IDebugBackend + MachBackend | Phase 0 interface, Phase 1 impl |
| `src/lldb` | `a64dbg_lldb` | LLDBBackend + ISymbolProvider | Phase 4 |
| `src/app` | `a64dbg` | main + MainWindow + theme | Phase 0 ✓ |
| `src/ui` | (tách khi view nhiều) | DisasmView/DumpView/... | Phase 2 |
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
