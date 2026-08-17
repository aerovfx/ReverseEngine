---
layout: course
title: "Week04"
permalink: /macos-reverse-engineering-10weeks/lessons/week04.html
---

# Tuần 04 — Static Analysis: Ghidra/Hopper, class-dump, strings, codesign

## Mục tiêu

- Dùng **Ghidra** (hoặc Hopper) để decompile và đọc luồng hàm.
- Trích **strings**, xem **symbol**, đọc **entitlements** và **code signature**.
- Lập báo cáo static analysis cho app mẫu (app của chính mình hoặc app mã nguồn mở).

## Công cụ và dữ liệu

- Ghidra (brew cask), Hopper (tuỳ chọn), `class-dump`, `strings`, `codesign`.
- App mẫu: app do chính bạn viết hoặc app mã nguồn mở đã build.

## Lý thuyết và ví dụ

**Static analysis** là đọc binary mà không chạy. Quy trình:

1. **Nhận dạng** — `file`, `otool -hv`, mã hash.
2. **Strings** — `strings binary` tìm chuỗi URL, key, message lỗi.
3. **Symbols** — `nm`, `class-dump` để khôi phục cấu trúc Objective-C.
4. **Decompile** — Ghidra chuyển assembly arm64 thành mã giả C.
5. **Code signing** — `codesign -dvvv app` xem bundle id, entitlements; `codesign -dv --entitlements - app` đọc quyền.

**Entitlements** cho biết app được phép làm gì (sandbox, network client, camera...). Đây là thông tin quan trọng khi hiểu hành vi app.

## Lab từng bước

1. Build app mẫu (hoặc dùng app mã nguồn mở đã build) và đặt trong VM lab.
2. Chạy `file`, `shasum -a 256`, `strings` trên binary; ghi lại chuỗi đáng chú ý.
3. Mở binary trong **Ghidra**; đọc `main`, tìm các hàm gọi API.
4. Chạy `class-dump` (nếu Objective-C) và `codesign -dvvv` + `--entitlements`.
5. Viết báo cáo static: hash, kiến trúc, strings, symbol, entitlements, luồng hàm chính.

## Liên kết code mẫu

Xem [code/week04/README.md](../code/week04/README.md) — `strings_extract.py`, `codesign_inspect.sh`.

## Thảo luận

- Vì sao cần đọc entitlements trước khi phân tích sâu?
- Ghidra so với Hopper khác nhau thế nào về khả năng decompile arm64?

## Bài tập

- **Cơ bản:** Liệt kê các bước static analysis và công cụ dùng cho từng bước.
- **Nâng cao:** Dùng Ghidra decompile `main` của app bạn viết và giải thích luồng.
- **Thử thách:** Viết báo cáo static hoàn chỉnh cho một app mã nguồn mở, kèm bằng chứng output.

## Nộp bài và rubric

Nộp báo cáo static + output script/codesign. Rubric chuẩn 100 điểm.

> **Cảnh báo pháp lý:** chỉ phân tích static app của chính bạn, app được cấp phép, hoặc app mã nguồn mở. Không dùng Ghidra/class-dump để trích logic app thương mại nhằm crack/vi phạm bản quyền. Tuần 05 chuyển sang dynamic analysis.
