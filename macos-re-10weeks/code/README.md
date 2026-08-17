# Code lab — macOS Reverse Engineering

Toàn bộ script trong `code/week01/..week10/` chạy độc lập bằng Python 3 / Bash (không cần thư viện ngoài), dùng **dữ liệu giả hoặc file do chính bạn tạo**. Không script nào đọc hoặc tải mẫu malware thật; các demo hook dylib/injection chỉ chạy trên app do chính bạn viết trong VM lab.

## Cách chạy

```bash
python3 code/weekNN/ten_script.py          # demo (mặc định)
python3 code/weekNN/ten_script.py <file>  # nếu script nhận file
bash code/weekNN/ten_script.sh            # script Bash
```

## Nguyên tắc an toàn và pháp lý

- Chỉ phân tích file/app do chính bạn tạo, app được cấp phép, hoặc app mã nguồn mở — trong VM cô lập.
- **Cấm** dùng code để patch app của bên thứ ba, né DRM/license để lậu, hoặc phát tán crack.
- Không chạy malware trên máy chính; không kết nối mạng thật khi chạy mẫu.
- Không commit/tải mẫu malware; chỉ giữ link tài liệu.
- Mỗi script có header chú thích mục tiêu/đầu vào/đầu ra/an toàn.

## Danh sách script

| Tuần | Script | Chức năng |
|---|---|---|
| 01 | `macos_env_check.sh` | Checklist môi trường lab macOS |
| 02 | `macho_parser.py` | Parse header Mach-O giả |
| 02 | `fat_detector.py` | Nhận diện universal binary |
| 03 | `objc_meta.py` | Mô phỏng metadata Objective-C |
| 03 | `swift_demangle_demo.py` | Minh hoạ giải mã symbol Swift |
| 04 | `strings_extract.py` | Trích strings ASCII/UTF-8 |
| 04 | `codesign_inspect.sh` | Đọc code signature + entitlements |
| 05 | `frida_hook_template.js` | Template hook Frida |
| 05 | `dynamic_log_parser.py` | Phân tích log dynamic giả |
| 06 | `swizzle_demo.m` | Demo method swizzling (ObjC) |
| 06 | `dylib_inject_demo.m` | Demo dylib injection (app của bạn) |
| 06 | `frida_objc_hook.js` | Hook method ObjC bằng Frida |
| 07 | `launchagent_gen.py` | Sinh LaunchAgent plist mẫu |
| 07 | `plist_parse.py` | Parse plist đơn giản |
| 08 | `ioc_report.py` | Sinh khung báo cáo IOC |
| 08 | `launchagent_scan.sh` | Quét LaunchAgents |
| 09 | `license_check.c` | App kiểm tra license (app của bạn) |
| 09 | `frida_license_hook.js` | Hook hàm license bằng Frida |
| 10 | `report_builder.py` | Sinh khung báo cáo RE |
| 10 | `rechecklist.sh` | Checklist RE tổng hợp |

Xem chi tiết từng tuần trong `WEEKLY_EXAMPLES.md` và README của từng thư mục.
