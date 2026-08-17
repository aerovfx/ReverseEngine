---
layout: course
title: "Components"
permalink: /macos-reverse-engineering-10weeks/references/components.html
---

# Components — Các thành phần của khoá học

## Môi trường và nền tảng

- **Apple Silicon (ARM64)** — kiến trúc mục tiêu chính của khoá.
- **macOS VM** — môi trường lab cô lập (UTM, Parallels, hoặc macOS VM từ IPSW).
- **SIP / AMFI** — lớp bảo vệ hệ thống; vô hiệu **chỉ trong VM lab**.

## Các lớp kiến thức

| Tuần | Thành phần | Nội dung chính |
|---|---|---|
| 01 | Môi trường lab | VM, SIP/AMFI, Homebrew, công cụ RE |
| 02 | Định dạng file | Mach-O, universal binaries, otool/nm/lipo |
| 03 | Nội bộ macOS | dyld, Objective-C/Swift runtime, TCC, sandbox |
| 04 | Static analysis | Ghidra/Hopper, class-dump, strings, codesign |
| 05 | Dynamic analysis | LLDB, Frida, fs_usage, unified logging |
| 06 | Runtime & hooking | method swizzling, dylib injection, Frida |
| 07 | Network & persistence | LaunchAgents, Wireshark, Little Snitch |
| 08 | RE malware mẫu 1 | XCSSET/OSX.Dok trong VM |
| 09 | RE app thương mại hợp pháp | cơ chế license, patch nghiên cứu |
| 10 | RE malware mẫu 2 + final | EvilQuest, tổng kết |

## Quy trình phân tích (dùng lại mọi tuần)

1. Nhận dạng (hash, file, otool).
2. Static (strings, symbol, Ghidra, entitlements).
3. Dynamic (LLDB/Frida, fs_usage, Wireshark).
4. Runtime/hooking (swizzle/dylib/Frida).
5. Persistence (launchctl, LaunchAgents).
6. IOC + phòng thủ.

## Tài nguyên học thêm

- Tài liệu Apple: Mach-O Programming Guide, dyld, Objective-C Runtime.
- Công cụ: Ghidra, Hopper, radare2/rizin, Frida, LLDB.
- Nguồn malware hợp pháp: chỉ mẫu do giảng viên/người sở hữu cấp.
