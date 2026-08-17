# Lộ trình macOS Reverse Engineering — 10 tuần (ARM64)

| Tuần | Năng lực chính | Lab local | Sản phẩm nộp | Nối tiếp |
|---:|---|---|---|---|
| 01 | Môi trường lab macOS, SIP/AMFI, cài công cụ | Dựng VM Apple Silicon + cài công cụ qua brew | Mô tả lab + checklist | Nền tảng cho mọi lab sau |
| 02 | Mach-O & universal binaries | Parse Mach-O bằng otool/nm/lipo trên app tự viết | Bảng phân tích Mach-O | Hiểu Mach-O để static analysis |
| 03 | macOS internals: dyld, ObjC/Swift, TCC, sandbox | Quan sát dyld, đọc metadata ObjC/Swift | Bảng so sánh các runtime | Nền tảng cho hooking |
| 04 | Static analysis | Ghidra/Hopper, class-dump, strings, codesign | Báo cáo static app mẫu | Đầu vào cho dynamic |
| 05 | Dynamic analysis | LLDB, Frida, fs_usage, log stream | Báo cáo dynamic app mẫu | Hiểu hành vi runtime |
| 06 | Runtime & hooking | Method swizzling, dylib injection, Frida | Demo hook hợp pháp | Kỹ thuật RE nâng cao |
| 07 | Network & persistence | LaunchAgents, Wireshark, Little Snitch | Báo cáo network/persistence | Bức tranh toàn diện app |
| 08 | RE malware macOS mẫu 1 | XCSSET/OSX.Dok trong VM cô lập | Báo cáo RE mẫu 1 | Kỹ thuật RE malware |
| 09 | RE app thương mại hợp pháp | Phân tích cơ chế license, patch nghiên cứu | Báo cáo RE license | Ứng dụng RE thực tế |
| 10 | RE malware macOS mẫu 2 + tổng kết | EvilQuest trong VM; final project | Báo cáo RE mẫu 2 + final project | Lộ trình cá nhân tiếp theo |

## Cách đánh giá

Mỗi tuần dùng rubric 100 điểm: đúng chức năng 35, an toàn và xử lý lỗi 25, chất lượng code/tài liệu 20, phân tích và bằng chứng chạy 20.

## Quy tắc lab và pháp lý

- Mọi phân tích chạy trong **VM cô lập**, snapshot trước khi chạy mẫu; giảm SIP/AMFI **chỉ trong lab**.
- Mạng giả lập hoặc tắt mạng; không kết nối nơi thật.
- **RE app thương mại chỉ trong phạm vi hợp pháp** (app của mình / được phép / mã nguồn mở); cấm phát tán crack, né DRM để lậu.
- Xoá snapshot sau khi hoàn tất; không giữ mẫu trên máy chính.
