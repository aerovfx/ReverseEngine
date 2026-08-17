---
layout: course
title: "Week10"
permalink: /software-cracking-10weeks/lessons/week10.html
---

# Tuần 10 — Crack serial key & dự án cuối khóa

## Mục tiêu

- Dùng breakpoint trên chuỗi để lần tới thuật toán kiểm tra serial.
- Hiểu khái niệm keygen (tạo serial hợp lệ) trong phạm vi hợp pháp.
- Hoàn thiện dự án cuối khóa (phân tích crackme hoặc app của bạn).

## Công cụ và dữ liệu

- x64dbg, crackme có kiểm tra serial.
- Script `code/week10/serial_check.py` (mô phỏng thuật toán kiểm tra serial + keygen cho app của bạn).

## Lý thuyết

Quy trình tìm serial:

1. Mở **String references**, tìm chuỗi "invalid"/"correct".
2. Đặt BP trên chuỗi, chạy, nhập serial giả.
3. Lần ngược tới lệnh `CMP` so sánh serial nhập với giá trị đúng.
4. Đọc giá trị đúng (trong thanh ghi/bộ nhớ) hoặc suy ra thuật toán.

**Keygen** là chương trình sinh serial đúng theo thuật toán. Viết keygen cho **app của chính bạn** (hoặc crackme) là bài tập hợp pháp để hiểu vì sao kiểm tra serial "client-side" yếu — từ đó biết cách bảo vệ (chuyển kiểm tra lên server, ký số).

## Lab từng bước

1. Nạp crackme, tìm chuỗi "invalid", đặt BP.
2. Chạy với serial giả, dừng tại BP, lần tới `CMP`.
3. Đọc giá trị serial đúng từ thanh ghi/bộ nhớ.
4. Chạy `python serial_check.py` để thấy mô phỏng thuật toán + keygen.
5. Hoàn thiện final project theo [projects/final_project.md](../projects/final_project.md).

## Liên kết code mẫu

Xem [code/week10/README.md](../code/week10/README.md) — script `serial_check.py`.

## Thảo luận

- Vì sao kiểm tra serial client-side dễ bị bypass?
- Làm thế nào để bảo vệ app của bạn (server-side check, signing)?

## Bài tập

- **Cơ bản:** Tìm serial đúng của một crackme bằng BP trên chuỗi.
- **Nâng cao:** Viết keygen cho thuật toán đơn giản của app bạn.
- **Thử thách:** Đề xuất 3 cách chống keygen/patch cho app của bạn.

## Rubric (100 điểm)

- Đúng chức năng (35): tìm đúng serial/keygen chạy được.
- An toàn (25): chỉ app của bạn/crackme.
- Tài liệu (20): quy trình + bằng chứng.
- Phân tích (20): giải thích thuật toán + đề xuất phòng thủ.

## An toàn & phạm vi

Keygen chỉ cho app của bạn/crackme. Không tạo/phát tán keygen cho phần mềm thương mại.
