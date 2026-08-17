---
layout: course
title: "Week02"
permalink: /macos-reverse-engineering-10weeks/lessons/week02.html
---

# Tuần 02 — Mach-O và Universal Binaries

## Mục tiêu

- Đọc cấu trúc **Mach-O** (header, load commands, segments).
- Nhận diện **universal binaries** (fat) và tách slice bằng `lipo`.
- Dùng `otool`, `nm`, `file` để trích thông tin từ file nhị phân macOS.

## Công cụ và dữ liệu

- `otool`, `nm`, `file`, `lipo`, `size` (đi kèm macOS / Xcode CLT).
- App/binary do chính bạn viết (`clang -o hello hello.c`) — không phân tích app thương mại của bên thứ ba trong bài này.
- Script `code/week02/macho_parser.py` (parse header giả).

## Lý thuyết và ví dụ

**Mach-O** là định dạng thực thi của macOS/iOS. Cấu trúc gồm:

- **Header** — magic (`0xFEEDFACE` 32-bit, `0xFEEDFACF` 64-bit, `0xCAFEBABE` fat), CPU type (arm64/x86_64), file type (MH_EXECUTE, MH_DYLIB, MH_BUNDLE...).
- **Load commands** — chỉ dẫn cho kernel/dyld: `LC_SEGMENT_64`, `LC_LOAD_DYLIB`, `LC_CODE_SIGNATURE`, `LC_MAIN`.
- **Segments** — vùng nhớ: `__TEXT` (code), `__DATA` (dữ liệu), `__LINKEDIT` (metadata cho dyld).

**Universal binary (fat)** chứa nhiều slice (arm64 + x86_64) trong một file; `lipo` tách/gộp các slice.

Ví dụ:
```bash
file hello
otool -hv hello            # header + load commands
otool -l hello             # chi tiết load commands
nm hello                   # symbol table
lipo -info hello
```

## Lab từng bước

1. Viết `hello.c` đơn giản và biên dịch: `clang -o hello hello.c`.
2. Chạy `file hello`, `otool -hv hello` — đọc magic, CPU type, load commands.
3. Chạy `nm hello` — quan sát symbol (đối chiếu với source).
4. Dùng `lipo -info` trên một universal binary hệ thống (ví dụ `/bin/ls`).
5. Chạy `python3 code/week02/macho_parser.py` trên một file giả để thấy luồng parse.

## Liên kết code mẫu

Xem [code/week02/README.md](../code/week02/README.md) — `macho_parser.py`, `fat_detector.py`.

## Thảo luận

- Vì sao cần phân biệt `__TEXT` và `__DATA` khi reverse engineering?
- Ưu điểm của universal binary so với file đơn kiến trúc là gì?

## Bài tập

- **Cơ bản:** Liệt kê các load command quan trọng và chức năng của chúng.
- **Nâng cao:** Dùng `otool -l` để tìm các dylib mà binary phụ thuộc và giải thích ý nghĩa.
- **Thử thách:** Viết một file Mach-O giả bằng tay (theo format) và parse nó bằng script của bạn.

## Nộp bài và rubric

Nộp bảng phân tích Mach-O của `hello` (header, load commands, segments) + output script. Rubric chuẩn 100 điểm.

> **Cảnh báo pháp lý:** chỉ phân tích binary do chính bạn tạo hoặc được cấp phép. Không dùng kỹ thuật đọc Mach-O để lấy nội dung app thương mại nhằm mục đích vi phạm bản quyền. Tuần 03 đi sâu vào nội bộ macOS.
