# WEEKLY_EXAMPLES — macOS Reverse Engineering

Mỗi tuần có script minh hoạ chạy độc lập. Dưới đây là mục đích và cách chạy từng script. **Luôn chạy trên dữ liệu giả hoặc app do chính bạn viết, trong VM lab.**

## Tuần 01 — `macos_env_check.sh`
Kiểm tra môi trường (Homebrew, Xcode CLT, công cụ RE) và sinh checklist.
```bash
bash code/week01/macos_env_check.sh
```

## Tuần 02 — `macho_parser.py`, `fat_detector.py`
Parse header Mach-O giả; nhận diện fat binary từ magic bytes.
```bash
python3 code/week02/macho_parser.py
python3 code/week02/fat_detector.py samples/sample_hello_macho.bin
```

## Tuần 03 — `objc_meta.py`, `swift_demangle_demo.py`
Mô phỏng metadata Objective-C; minh hoạ giải mã symbol Swift mangled.
```bash
python3 code/week03/objc_meta.py
python3 code/week03/swift_demangle_demo.py
```

## Tuần 04 — `strings_extract.py`, `codesign_inspect.sh`
Trích strings ASCII/UTF-8; đọc code signature + entitlements của app của bạn.
```bash
python3 code/week04/strings_extract.py /path/to/your/binary
bash code/week04/codesign_inspect.sh /path/to/your.app
```

## Tuần 05 — `frida_hook_template.js`, `dynamic_log_parser.py`
Template hook Frida; parse log dynamic giả.
```bash
frida -n YourApp -l code/week05/frida_hook_template.js
python3 code/week05/dynamic_log_parser.py samples/dyn.log
```

## Tuần 06 — `swizzle_demo.m`, `dylib_inject_demo.m`, `frida_objc_hook.js`
Demo swizzling và dylib injection trên app của bạn; hook ObjC bằng Frida.
```bash
clang -framework Foundation code/week06/swizzle_demo.m -o /tmp/swizzle && /tmp/swizzle
```

## Tuần 07 — `launchagent_gen.py`, `plist_parse.py`
Sinh LaunchAgent plist mẫu; parse plist đơn giản.
```bash
python3 code/week07/launchagent_gen.py com.example.app
python3 code/week07/plist_parse.py /path/to/plist
```

## Tuần 08 — `ioc_report.py`, `launchagent_scan.sh`
Sinh khung báo cáo IOC; quét LaunchAgents.
```bash
python3 code/week08/ioc_report.py
bash code/week08/launchagent_scan.sh
```

## Tuần 09 — `license_check.c`, `frida_license_hook.js`
App kiểm tra license (của bạn) + hook Frida. Chỉ dùng trên app của bạn.
```bash
clang code/week09/license_check.c -o /tmp/license && /tmp/license SECRETKEY123
frida -n license -l code/week09/frida_license_hook.js
```

## Tuần 10 — `report_builder.py`, `rechecklist.sh`
Sinh khung báo cáo RE; checklist RE tổng hợp.
```bash
python3 code/week10/report_builder.py
bash code/week10/rechecklist.sh
```
