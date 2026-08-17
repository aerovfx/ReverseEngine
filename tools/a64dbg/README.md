# a64dbg — debugger cho macOS ARM64 (x64dbg-like)

Trình gỡ lỗi (debugger) GUI native chạy trên Apple Silicon, mô phỏng trải nghiệm x64dbg (Windows):
view CPU/disassembly, registers, memory dump, stack, breakpoints (phần mềm/phần cứng/watchpoint),
stepping, symbolication, plugin SDK và scripting.

> **Phạm vi hợp pháp:** công cụ dùng cho học reverse engineering, phân tích malware trong lab cô lập
> và gỡ lỗi phần mềm của chính mình. Attach tiến trình khác cần entitlement
> `com.apple.security.cs.debugger` + quyền hợp lệ; SIP chặn attach tiến trình Apple ký hệ thống.

## Tính năng (đã hoàn thành)

| Nhóm | Tính năng |
|---|---|
| Phiên | attach/detach (`task_for_pid`), suspend/resume |
| Breakpoint | software (BRK), hardware (bvr/bcr), watchpoint (wvr/wcr), breakpoint-dance |
| Step | step into (hardware SS), step over (disasm + temp bp), step out (temp bp tại LR) |
| Bộ nhớ/Register | read/write (`mach_vm_*`), `ARM_THREAD_STATE64` |
| Disassembly | Capstone ARM64 |
| Symbolication | native (`TASK_DYLD_INFO` + `LC_SYMTAB`) |
| View | CPU/disasm, registers (34 thanh ghi), dump (hex+ASCII), stack |
| Tìm kiếm/patch | byte pattern, patch bytes, nop fill |
| Mở rộng | plugin SDK (C ABI, dlopen), scripting (QJSEngine) |

Kiến trúc chi tiết: [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md). Kế hoạch: [`../../plans/build-macos-debugger.md`](../../plans/build-macos-debugger.md).

## Build

Yêu cầu: macOS arm64, Xcode (clang), CMake ≥ 3.24, Ninja, Qt 6 (brew), mạng (FetchContent Capstone).

```bash
# Debug
cmake --preset mac-arm64-debug
cmake --build --preset mac-arm64-debug
ctest --preset mac-arm64-debug        # 12 test tự động

# Release + ký entitlement
cmake --preset mac-arm64-release
cmake --build --preset mac-arm64-release
cmake --build --preset mac-arm64-release --target sign-a64dbg
```

Ứng dụng: `build/mac-arm64-release/src/app/a64dbg.app` (hoặc `build/mac-arm64-debug/...`).

Đóng gói `.dmg`: `./scripts/package.sh` (cần chạy ngoài sandbox).

## Dùng

1. Mở `a64dbg.app`.
2. **File → Attach** → nhập PID của tiến trình muốn gỡ lỗi.
3. **Debug → Run/Continue** / **Step Into**.
4. Xem disassembly (CPU), registers, dump; đặt breakpoint qua plugin/script (GUI thêm bp ở Phase sau).

## Plugin

Viết plugin dạng loadable module (C ABI `include/dbg_sdk.h`), export `pluginit`/`plugstop`/`plugsetup`.
Xem mẫu [`plugins/example/`](plugins/example/) và tài liệu [`docs/PLUGINS.md`](docs/PLUGINS.md).

## Kiểm thử

```bash
ctest --preset mac-arm64-debug
# capstone_smoke · disasm · macho · mach_backend · controller ·
# phase3 · symbol · phase4 · phase5 · plugin · plugin_integration · script
```

## Ghi chú nền tảng (đã đo trên máy thật)

Xem mục "Gotcha nền tảng ARM64" trong [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md):
W^X khi patch code, BRK/single-step/hw-bp/watchpoint đều gửi `EXC_BREAKPOINT` với `code` khác nhau,
phải cộng ASLR slide khi đọc symbol table, và detach phải khôi phục breakpoint trước khi resume.
