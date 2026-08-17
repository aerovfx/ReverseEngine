---
layout: course
title: "Week07"
permalink: /software-cracking-10weeks/lessons/week07.html
---

# Tuần 07 — Reversing jumps

## Mục tiêu

- Đảo một lệnh nhảy có điều kiện để luồng đi nhánh khác.
- Hiểu cách thay đổi cờ ZF (1↔0) làm nhảy "taken/not taken".
- Bypass thông báo "try harder" để quan sát nhánh ẩn.

## Công cụ và dữ liệu

- x64dbg, crackme có chuỗi "try harder"/"invalid".
- Script `code/week07/reverse_jump.py` (mô phỏng đảo nhảy trên luồng giả).

## Lý thuyết

Một chương trình kiểm tra serial thường có dạng:

```
CMP input, serial      ; so sánh
JE  success            ; nếu bằng -> nhảy tới "đúng"
; ngược lại -> in "try harder"
```

Khi `input != serial`, ZF=0, `JE` không nhảy → chương trình in "try harder". **Reversing jump** là làm cho nhảy xảy ra ngược lại: đổi `JE`→`JNE` (hoặc NOP lệnh nhảy, hoặc sửa cờ ZF) để luồng đi vào nhánh thành công. Đây là kỹ thuật **hiểu luồng**, dùng để xác nhận vị trí kiểm tra — không phải để phá khóa phần mềm thương mại.

## Lab từng bước

1. Nạp crackme, tìm chuỗi "try harder", đặt BP (tuần 06).
2. Chạy tới BP, lần lên lệnh `JE/JNE` quyết định nhánh.
3. Bấm đúp để sửa cờ ZF (hoặc đổi `JE`→`JNE`), quan sát nhảy thay đổi.
4. Step over để xác nhận luồng đi vào nhánh ẩn.
5. Ghi lại địa chỉ lệnh đã đảo và hành vi trước/sau.

## Liên kết code mẫu

Xem [code/week07/README.md](../code/week07/README.md) — script `reverse_jump.py` · `reverse_jump.cpp` (C++17 chi tiết)

## Thảo luận

- Đảo `JE` thành `JNE` khác gì NOP lệnh nhảy?
- Vì sao thay đổi này chỉ là tạm thời (mất khi restart)?

## Bài tập

- **Cơ bản:** Giải thích cặp `CMP`+`JE` quyết định nhánh như thế nào.
- **Nâng cao:** Đảo một nhảy trên crackme và chụp bằng chứng nhánh đổi.
- **Thử thách:** Tìm cách bypass mà không sửa lệnh nhảy (chỉ sửa cờ/thanh ghi).

## Rubric (100 điểm)

- Đúng chức năng (35): đảo nhảy thành công, có bằng chứng.
- An toàn (25): chỉ trên crackme/binary của bạn.
- Tài liệu (20): địa chỉ + hành vi trước/sau.
- Phân tích (20): giải thích đúng vai trò ZF.

## An toàn & phạm vi

Kỹ thuật này chỉ để học trên crackme/binary của bạn. Không áp dụng lên phần mềm thương mại của người khác.
