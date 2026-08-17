---
layout: course
title: "Week03"
permalink: /macos-reverse-engineering-10weeks/lessons/week03.html
---

# Tuần 03 — macOS Internals: dyld, Objective-C/Swift runtime, TCC, sandbox

## Mục tiêu

- Giải thích vai trò của **dyld** trong quá trình load binary.
- Hiểu **Objective-C runtime** (isa, objc_msgSend) và **Swift metadata**.
- Nắm khái niệm **TCC** (quyền riêng tư) và **sandbox**.

## Công cụ và dữ liệu

- `dyld_info`, `otool`, `nm`, `class-dump` (qua brew), `swift-demangle`.
- App Objective-C/Swift do chính bạn viết.

## Lý thuyết và ví dụ

**dyld** (dynamic linker) là thành phần load Mach-O vào bộ nhớ, resolve symbol, bind dylib. Hiểu dyld giúp biết app nạp thư viện nào, symbol nào được bind.

**Objective-C runtime**:
- Mỗi đối tượng có con trỏ `isa` trỏ tới class.
- Gọi method thực chất là `objc_msgSend(receiver, selector, ...)` — dispatch động.
- Class metadata lưu method lists → có thể đọc bằng `class-dump` và hook bằng **method swizzling** (tuần 06).

**Swift** dùng metadata và **mangled symbol** (`swift-demangle` để giải mã). Interop với Objective-C qua `@objc`.

**TCC** kiểm soát quyền riêng tư (camera, mic, file, screen recording). **Sandbox** giới hạn app trong phạm vi container. Khi RE app, cần biết app yêu cầu entitlement gì (tuần 04).

## Lab từng bước

1. Viết app Objective-C nhỏ (`@implementation` + 1 method) và biên dịch bằng `clang -framework Foundation`.
2. Dùng `nm` và `otool -L` để xem symbol và dylib phụ thuộc.
3. Chạy `class-dump` trên app của bạn — quan sát method list được tái dựng.
4. Dùng `swift-demangle` trên một symbol Swift mangled.
5. Chạy `dyld_info` (hoặc `otool -L`) để xem chuỗi load.

## Liên kết code mẫu

Xem [code/week03/README.md](../code/week03/README.md) — `objc_meta.py` (mô phỏng metadata ObjC), `swift_demangle_demo.py`.

## Thảo luận

- Vì sao Objective-C dễ reverse hơn C thuần (do metadata giàu thông tin)?
- TCC và sandbox ảnh hưởng thế nào đến khả năng hook một app?

## Bài tập

- **Cơ bản:** Giải thích vai trò của `isa`, `objc_msgSend`, method list trong Objective-C.
- **Nâng cao:** Dùng `class-dump` trên app của bạn và liệt kê các method không có trong source (nếu có).
- **Thử thách:** Viết mô phỏng `objc_msgSend` bằng Python và giải thích luồng dispatch động.

## Nộp bài và rubric

Nộp bảng so sánh runtime Objective-C vs Swift + output class-dump/swift-demangle. Rubric chuẩn 100 điểm.

> **Cảnh báo pháp lý:** chỉ chạy class-dump trên app của chính bạn hoặc app được cấp phép. Không dùng metadata ObjC/Swift để trích xuất logic của app thương mại nhằm vi phạm bản quyền. Tuần 04 bắt đầu static analysis.
