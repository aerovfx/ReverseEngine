---
layout: course
title: "Week06"
permalink: /software-cracking-10weeks/lessons/week06.html
---

# Tuần 06 — Breakpoints

## Mục tiêu

- Đặt breakpoint phần mềm (F2) và chạy tới breakpoint (F9).
- Tìm chuỗi bằng **string references**.
- Đặt breakpoint trên chuỗi gợi ý ("try harder", "invalid", "correct"...) để lần ngược luồng kiểm tra.

## Công cụ và dữ liệu

- x64dbg, crackme hoặc binary của bạn.
- Script `code/week06/breakpoint_sim.py` (mô phỏng engine breakpoint INT3).

## Lý thuyết

**Breakpoint** là điểm dừng chương trình tại một địa chỉ hoặc khi chạm một điều kiện:

- **Software BP** — ghi lệnh `INT3` (0xCC) vào vị trí, dừng khi CPU gặp nó.
- **Hardware BP** — dùng debug register, giới hạn số lượng (thường 4).
- **Memory BP** — dừng khi một vùng nhớ bị đọc/ghi.

Mẹo quan trọng: dùng **String references** (chuột phải → Search for → Current module → String references) để liệt kê mọi chuỗi trong binary. Đặt BP trên chuỗi gợi ý rồi chạy — khi chương trình chạm chuỗi đó, debugger dừng đúng chỗ, bạn lần ngược lên lệnh so sánh.

## Lab từng bước

1. Nạp crackme, mở String references, tìm chuỗi "try harder" (hoặc tương tự).
2. Nhảy tới tham chiếu của chuỗi, đặt breakpoint (F2).
3. Chạy (F9), nhập input bất kỳ, quan sát dừng tại BP.
4. Lần ngược lên lệnh `CMP`/`JE` phía trên.
5. Chạy `python breakpoint_sim.py` để hiểu cơ chế INT3.

## Liên kết code mẫu

Xem [code/week06/README.md](../code/week06/README.md) — script `breakpoint_sim.py`.

## Thảo luận

- Software BP khác hardware BP ở giới hạn nào?
- Vì sao đặt BP trên chuỗi lại hiệu quả khi tìm serial?

## Bài tập

- **Cơ bản:** Đặt 1 software BP và 1 BP trên chuỗi, mô tả kết quả.
- **Nâng cao:** Dùng string references lần tới lệnh so sánh serial.
- **Thử thách:** Giải thích vì sao hardware BP chỉ có ~4 điểm và khi nào nên dùng.

## Rubric (100 điểm)

- Đúng chức năng (35): đặt BP đúng, dừng đúng chỗ.
- An toàn (25): lab cô lập.
- Tài liệu (20): ảnh BP + chuỗi.
- Phân tích (20): lần ngược đúng lệnh so sánh.

## An toàn & phạm vi

Chỉ BP trên binary hợp pháp. Không đặt BP lên tiến trình hệ thống của người khác.
