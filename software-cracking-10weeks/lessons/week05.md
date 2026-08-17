---
layout: course
title: "Week05"
permalink: /software-cracking-10weeks/lessons/week05.html
---

# Tuần 05 — Stepping & call stack

## Mục tiêu

- Phân biệt step into (F7) và step over (F8).
- Đọc call stack để hiểu chuỗi lời gọi hàm.
- Theo dõi nhảy "taken / not taken" khi step.

## Công cụ và dữ liệu

- x64dbg, crackme hoặc binary của bạn.
- Script `code/week05/step_sim.py` (mô phỏng luồng lệnh và call).

## Lý thuyết

- **Step over (F8)** chạy trọn một lệnh; nếu lệnh là `CALL`, nó chạy cả hàm bên trong rồi quay lại, không đi sâu.
- **Step into (F7)** đi **vào bên trong** hàm được gọi.
- **Call stack** là chồng các khung gọi hàm: cho biết "ai gọi ai", giúp bạn lần ngược từ một hàm kiểm tra về nơi nó được gọi.

Khi muốn hiểu một hàm kiểm tra serial làm gì, bạn step into nó; khi muốn lướt nhanh qua code "ồn ào", bạn step over.

## Lab từng bước

1. Nạp crackme, chạy tới entry point.
2. Tìm một lệnh `CALL`, bấm F8 (step over) và quan sát: không đi sâu.
3. Restart, tới cùng `CALL`, bấm F7 (step into) và quan sát: vào trong hàm.
4. Mở cửa sổ call stack, ghi lại chuỗi lời gọi.
5. Chạy `python step_sim.py` để thấy mô phỏng tương đương.

## Liên kết code mẫu

Xem [code/week05/README.md](../code/week05/README.md) — script `step_sim.py`.

## Thảo luận

- Khi nào nên step into thay vì step over?
- Call stack giúp gì khi lần ngược nguồn gốc một lời gọi?

## Bài tập

- **Cơ bản:** Mô tả sự khác nhau F7/F8 bằng ví dụ.
- **Nâng cao:** Step vào một hàm, vẽ sơ đồ call stack.
- **Thử thách:** Tìm hàm in thông báo "invalid" bằng cách step và call stack.

## Rubric (100 điểm)

- Đúng chức năng (35): thao tác F7/F8 đúng, đọc được call stack.
- An toàn (25): lab cô lập.
- Tài liệu (20): sơ đồ call stack rõ.
- Phân tích (20): giải thích luồng gọi hàm.

## An toàn & phạm vi

Chỉ phân tích binary hợp pháp. Snapshot trước khi step sâu vào các lời gọi.
