---
layout: course
title: "Week04"
permalink: /software-cracking-10weeks/lessons/week04.html
---

# Tuần 04 — Bước cơ bản trong debugger

## Mục tiêu

- Nạp binary vào x64dbg, chạy tới entry point, step over (F8).
- Phân biệt jump có điều kiện và không điều kiện.
- Hiểu vai trò cờ ZF trong quyết định nhảy.

## Công cụ và dữ liệu

- x64dbg, một crackme hoặc binary của bạn.
- Script `code/week04/jump_sim.py` (mô phỏng CMP + JE + cờ ZF).

## Lý thuyết

Trong debugger, bạn thấy ba cột: **địa chỉ**, **mã máy (hex)**, **assembly**. Hai loại nhảy:

- `JMP` — nhảy **không điều kiện**, luôn nhảy.
- `JE/JNE/JZ/JNZ...` — nhảy **có điều kiện**, chỉ nhảy khi cờ phù hợp.

`CMP a, b` so sánh và đặt cờ **ZF** (zero flag): nếu `a == b` thì ZF=1, `JE` (jump if equal) sẽ nhảy. Khi gặp lệnh kiểm tra serial/password, thường có một `CMP` rồi một `JE/JNE` quyết định "đúng hay sai".

## Lab từng bước

1. Nạp crackme vào x64dbg, chạy tới entry point (F9 rồi dừng).
2. Nhấn F8 (step over) vài lệnh, quan sát con trỏ dịch chuyển.
3. Tìm một lệnh `JMP` và một lệnh `JE/JNE`, ghi địa chỉ.
4. Quan sát cờ ZF thay đổi khi qua `CMP`.
5. Chạy `python jump_sim.py` để thấy mô phỏng tương đương.

## Liên kết code mẫu

Xem [code/week04/README.md](../code/week04/README.md) — script `jump_sim.py` · `jump_sim.cpp` (C++17 chi tiết)

## Thảo luận

- Step over (F8) khác step into (F7) ở điểm nào?
- Nếu ZF=1 và gặp `JNE`, lệnh nhảy có xảy ra không?

## Bài tập

- **Cơ bản:** Liệt kê 5 lệnh nhảy có điều kiện và nghĩa của chúng.
- **Nâng cao:** Trên crackme, xác định 1 cặp `CMP`+`JE` và mô tả luồng.
- **Thử thách:** Dự đoán nhánh chạy khi đổi giá trị ZF bằng tay, rồi kiểm chứng.

## Rubric (100 điểm)

- Đúng chức năng (35): thao tác step over đúng, xác định đúng loại jump.
- An toàn (25): chỉ dùng binary hợp pháp.
- Tài liệu (20): ảnh/địa chỉ rõ ràng.
- Phân tích (20): giải thích đúng vai trò ZF.

## An toàn & phạm vi

Chỉ step trên crackme/binary của bạn trong VM. Không ghi đè binary gốc khi chưa backup.
