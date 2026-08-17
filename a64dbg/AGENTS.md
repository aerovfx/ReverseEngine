# a64dbg — trình gỡ lỗi (debugger) cho macOS ARM64, mô phỏng x64dbg

## Mục tiêu dự án

Xây một debugger GUI native chạy trên Apple Silicon, có trải nghiệm tương tự x64dbg (Windows): view CPU/disassembly, registers, memory dump (hex+ASCII), stack, breakpoints (phần mềm/phần cứng/watchpoint), stepping, symbol, plugin SDK và scripting. Tên làm việc: **a64dbg**.

- Kiến trúc đích: **ARM64 (AArch64)** trước; x86_64/Rosetta để dành.
- Backend **hai tầng**: tự viết core bằng **Mach API**, tái dùng **LLDB SB API** cho symbolication/step phức tạp/`expr`.
- Phạm vi: đầy đủ như x64dbg, triển khai theo 8 phase (xem `docs/ARCHITECTURE.md` và `plans/build-macos-debugger.md`).

## Tech stack

- **C++20**, GUI **Qt 6 (Widgets)**, build **CMake + Ninja** (presets).
- Disassembler **Capstone** (CS_ARCH_ARM64), assembler **Keystone** (Phase 5), emulator **Unicorn** (tùy chọn, fallback step).
- Debug core: Mach API hệ thống — `task_for_pid`, Mach exception ports, `thread_get/set_state` (`ARM_THREAD_STATE64`, `ARM_DEBUG_STATE64`), `mach_vm_*`.
- Symbol/step: LLDB SB API (`SBProcess`/`SBThread`/`SBFrame`).
- Plugin: C ABI + `dlopen` (header `include/dbg_sdk.h`). Scripting: Lua (Phase 6).
- Dependency: CMake `FetchContent` ghim tag cho thư viện biên dịch (Capstone...).

## Kiến trúc & quy tắc bắt buộc

1. **Tách backend khỏi GUI**: GUI chỉ nói chuyện qua interface `IDebugBackend` (`src/core/IDebugBackend.h`) và `ISymbolProvider`. Không import header Mach/LLDB vào thư mục `src/app/` hoặc `src/ui/`.
2. **Một-thư-mục-một-chủ**: `common` (log/version), `model` (session/breakpoint state), `disasm` (Capstone), `core` (Mach backend), `lldb` (LLDB backend), `app` (main + MainWindow), `ui` (các view). Mỗi cái là một target CMake riêng; cấm import chéo vòng.
3. **Event bất đồng bộ**: thread riêng chạy vòng `mach_msg`, chuẩn hóa thành `DebugEvent`, đẩy vào hàng đợi thread-safe; GUI nhận qua Qt queued connection. Không gọi Mach API trực tiếp từ GUI thread khi đang dừng (deadlock).
4. **Stop-the-world**: khi một thread dừng do exception, phải suspend đủ các thread còn lại trước khi đọc state.
5. **ARM64 đúng chuẩn**: không giả định encoding x86; chú ý PC-relative khác x86; `pc`/`cpsr` đúng ngữ nghĩa AArch64.
6. **Không đổi kiến trúc giữa chừng** nếu chưa xin xác nhận người dùng.

## Coding style

- Namespace `a64dbg` (lồng `a64dbg::core`, `a64dbg::disasm`...). File header dùng `#pragma once`.
- Tên class `PascalCase`, hàm `camelCase`, biến thành viên `m_` (vd `m_disasmView`).
- Header `<cstdint>` cho kiểu độ rộng cố định; địa chỉ dùng `std::uint64_t`.
- Không `using namespace std` trong header; `auto` khi kiểu hiển nhiên; `const` đúng mức.
- Comment tiếng Việt ngắn gọn ở chỗ quan trọng; thuật ngữ chuyên môn giữ tiếng Anh.

## UI/UX guideline

- Dark theme mặc định (Fusion + palette tối), giống x64dbg.
- Layout dock: **CPU** (trên-trái) · **Registers** (phải) · **Stack** (dưới-trái) · **Dump** (dưới-phải). Stack và Dump tab hóa.
- Địa chỉ hiển thị hex 16 chữ số; breakpoint có highlight; cross-navigation "follow in dump / follow in disasm".
- Không chặn luồng UI bằng thao tác đọc memory lớn — đọc theo chunk/async.

## Review & chất lượng

- Mỗi phase kết thúc bằng build xanh + `ctest` + tự kiểm (B2) trước khi sang phase sau.
- Giai đoạn C (đánh giá độc lập + phản biện chéo) là bắt buộc trước khi phát hành — hai reviewer độc lập, báo cáo lưu `reports/`.
- Cảnh báo bật `-Wall -Wextra -Wpedantic -Wshadow` cho target của mình (không áp lên dependency).

## Pháp lý & an toàn

- Chỉ xây công cụ debug hợp pháp phục vụ học reverse engineering, gỡ lỗi phần mềm của chính mình, phân tích malware trong lab cô lập.
- KHÔNG kèm hướng dẫn crack/phá DRM/license trái phép; không né SIP/AMFI/TCC cho mục đích lậu.
- Attach vào tiến trình người khác cần entitlement `com.apple.security.cs.debugger` + quyền hợp lệ; SIP chặn attach tiến trình Apple ký hệ thống (giới hạn nền tảng, không tìm cách vượt).
