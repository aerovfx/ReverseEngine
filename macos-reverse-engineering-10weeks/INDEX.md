---
layout: course
title: "Index"
permalink: /macos-reverse-engineering-10weeks/INDEX.html
---

# macOS Reverse Engineering — ReverseEngine 10 tuần (ARM64)

Khoá học 10 tuần về **reverse engineering ứng dụng trên macOS**, chạy trên máy **Apple Silicon (ARM64)**, theo chuẩn ReverseEngine: bài học, code minh hoạ chạy được, bài tập 3 mức và dự án cuối khoá. Nội dung thiên về **phân tích và hiểu cơ chế hoạt động của ứng dụng** (bao gồm cơ chế cấp phép/license) — luôn trong phạm vi hợp pháp.

## Kết quả đầu ra

Sau 10 tuần, người học có thể:

- Dựng môi trường lab macOS an toàn (VM Apple Silicon, SIP/AMFI, snapshot, cài công cụ qua Homebrew).
- Đọc cấu trúc **Mach-O** và universal binaries; dùng `otool`, `nm`, `lipo`.
- Giải thích nội bộ macOS: **dyld**, **Objective-C/Swift runtime**, TCC, sandbox, code signing.
- Thực hiện **static analysis** bằng Ghidra/Hopper, class-dump, strings, `codesign`/entitlements.
- Thực hiện **dynamic analysis** bằng LLDB, Frida, `fs_usage`, unified logging.
- Hook runtime bằng **method swizzling**, **dylib injection**, script Frida.
- Phân tích cơ chế cấp phép của ứng dụng và hiểu cách patch để **nghiên cứu** (hợp pháp).
- Phân tích malware macOS thực tế (XCSSET/OSX.Dok, EvilQuest) trong lab cô lập.

## Cấu trúc

- 10 bài học tăng dần từ môi trường đến RE ứng dụng nâng cao.
- 10 bộ code minh hoạ Python/Bash chạy độc lập, dữ liệu giả/lab local an toàn.
- Bài tập cơ bản, nâng cao và thử thách mỗi tuần.
- Rubric 100 điểm và dự án cuối khoá (phân tích app của chính mình hoặc app mã nguồn mở).

## Quy tắc an toàn và pháp lý (BẮT BUỘC)

> Khoá học này chỉ dạy kỹ thuật RE trong **phạm vi hợp pháp**: phân tích **ứng dụng của chính bạn**, ứng dụng **được cấp phép** cho mục đích nghiên cứu, hoặc ứng dụng **mã nguồn mở**. **Cấm** phát tán bản crack, né DRM/license để dùng lậu, hoặc dùng kỹ thuật để vi phạm bản quyền. Mọi phân tích malware chạy trong VM cô lập, mạng giả lập, không bao giờ trên máy chính. Xem [references/safety.md](references/safety.md) và [references/software.md](references/software.md).
