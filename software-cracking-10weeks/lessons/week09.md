---
layout: course
title: "Week09"
permalink: /software-cracking-10weeks/lessons/week09.html
---

# Tuần 09 — Cracking GUI & phân tích PE

## Mục tiêu

- Nhận diện chương trình GUI (Windows API: MessageBox, GetWindowText).
- Đặt breakpoint trên API để bắt luồng nhập serial.
- Phân tích PE của chương trình GUI (import table, section).

## Công cụ và dữ liệu

- x64dbg, crackme GUI hoặc app bạn tự viết.
- Script `code/week09/winapi_sim.py` (mô phỏng luồng MessageBox/GetWindowText + kiểm tra serial).

## Lý thuyết

Chương trình GUI đọc input qua API như `GetWindowTextA/W`, hiển thị kết quả qua `MessageBoxA/W`. Khi crack GUI:

1. Đặt breakpoint trên `GetWindowTextA/W` để bắt nơi đọc chuỗi người dùng nhập.
2. Đặt breakpoint trên `MessageBoxA/W` để bắt nơi hiển thị "đúng/sai".
3. Lần ngược từ đó tới lệnh so sánh serial.

Phân tích PE (import table) cho biết chương trình dùng những API nào → chọn đúng BP. Đây là bước "triage" nâng cao trước khi debug.

## Lab từng bước

1. Nạp crackme GUI, mở tab Symbols/Imports, tìm `GetWindowTextA` và `MessageBoxA`.
2. Đặt BP trên `GetWindowTextA`, chạy, nhập input, quan sát dừng.
3. Đặt BP trên `MessageBoxA`, chạy tới khi hiển thị "sai".
4. Lần ngược tới lệnh `CMP` so sánh serial.
5. Chạy `python winapi_sim.py` để thấy mô phỏng luồng tương đương.

## Liên kết code mẫu

Xem [code/week09/README.md](../code/week09/README.md) — script `winapi_sim.py`.

## Thảo luận

- Vì sao BP trên API hiệu quả với chương trình GUI?
- Import table giúp gì trước khi đặt BP?

## Bài tập

- **Cơ bản:** Liệt kê 3 API thường gặp khi crack GUI và vai trò.
- **Nâng cao:** Đặt BP trên `GetWindowTextA`, lần tới lệnh so sánh.
- **Thử thách:** Phân tích import table, dự đoán luồng kiểm tra trước khi chạy.

## Rubric (100 điểm)

- Đúng chức năng (35): BP trên API đúng, lần được tới so sánh.
- An toàn (25): chỉ binary hợp pháp.
- Tài liệu (20): import table + ảnh BP.
- Phân tích (20): giải thích luồng GUI.

## An toàn & phạm vi

Chỉ phân tích GUI của bạn/crackme. Không hook API của tiến trình người khác.
