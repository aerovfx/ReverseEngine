---
layout: course
title: "Week09"
permalink: /macos-reverse-engineering-10weeks/lessons/week09.html
---

# Tuần 09 — RE ứng dụng thương mại hợp pháp: cơ chế license, patch nghiên cứu

## Mục tiêu

- Hiểu các cơ chế cấp phép (license) thường gặp trên macOS.
- Phân tích **app của chính mình** có cơ chế license để học cách hoạt động.
- Thực hành patch để **nghiên cứu** chỉ trên app của mình, trong phạm vi hợp pháp.

## Công cụ và dữ liệu

- Ghidra, LLDB, Frida.
- App license do **chính bạn viết** (một app đơn giản kiểm tra serial) — không dùng app thương mại của bên thứ ba.

## Lý thuyết và ví dụ

**Cơ chế license** thường gặp:
- Kiểm tra serial/key đơn giản (so sánh chuỗi, hash).
- Kiểm tra ngày hết hạn (date check).
- Gọi server xác thực (online licensing).
- Ký số / dùng API như StoreKit.

Khi RE, ta tìm **điểm quyết định** (branch) — hàm trả về "hợp lệ/không hợp lệ". Với app của mình, có thể:
- Dùng Ghidra tìm hàm kiểm tra.
- Dùng LLDB đặt breakpoint thay đổi giá trị trả về.
- Dùng Frida hook hàm kiểm tra để luôn trả "hợp lệ".

**Phạm vi hợp pháp (bắt buộc):** chỉ thực hành trên app do **chính bạn viết** hoặc app **được cấp phép** cho mục đích nghiên cứu. Việc patch app của người khác để dùng lậu là **vi phạm bản quyền/DMCA** và không được hướng dẫn trong khoá này.

## Lab từng bước

1. Viết app C/Objective-C có hàm `int check_license(char *key)` trả 1/0.
2. Dùng Ghidra tìm hàm và xác định nhánh quyết định.
3. Dùng LLDB đặt breakpoint ở hàm, đọc tham số, thay đổi giá trị trả về.
4. Viết script Frida hook `check_license` để luôn trả về hợp lệ.
5. Ghi chú rõ: kỹ thuật nào dùng trên app của mình, kỹ thuật nào KHÔNG được dùng trên app của người khác.

## Liên kết code mẫu

Xem [code/week09/README.md](../code/week09/README.md) — `license_check.c`, `frida_license_hook.js` · `license_check.cpp` (C++17 chi tiết)

## Thảo luận

- Vì sao hiểu cơ chế license giúp bảo vệ app của chính bạn?
- Đâu là ranh giới giữa nghiên cứu hợp pháp và vi phạm bản quyền?

## Bài tập

- **Cơ bản:** Liệt kê các cơ chế license phổ biến trên macOS.
- **Nâng cao:** Dùng LLDB thay đổi kết quả `check_license` trên app của bạn.
- **Thử thách:** Viết script Frida hook `check_license` và giải thích cách phòng thủ nếu đây là app của bạn.

## Nộp bài và rubric

Nộp báo cáo RE license (app của bạn) + script + bằng chứng chạy. Rubric chuẩn 100 điểm.

> **Cảnh báo pháp lý (bắt buộc):** khoá học **chỉ** thực hành RE license trên app do chính bạn viết hoặc app được cấp phép cho nghiên cứu. **Cấm** patch app của bên thứ ba để dùng lậu, né DRM, hoặc phát tán bản crack — hành vi đó vi phạm bản quyền/DMCA. Tuần 10 phân tích malware mẫu 2 và tổng kết.
