---
layout: course
title: "Week02"
permalink: /software-cracking-10weeks/lessons/week02.html
---

# Tuần 02 — x64dbg & Detect It Easy (DIE)

## Mục tiêu

- Cài và chạy được x64dbg (32/64-bit) và DIE trên Windows.
- Xử lý lỗi thiếu `api-ms-win-crt-runtime-l1-1-0.dll`.
- Dùng DIE để nhận diện compiler/packer của một binary.

## Công cụ và dữ liệu

- x64dbg (x64dbg.com), DIE (ntinfo.biz), Microsoft Visual C++ Redistributable.
- Script `code/week02/die_sim.py` (mô phỏng nhận diện signature, không cần DIE).

## Lý thuyết

**x64dbg** là debugger mã nguồn mở: nạp binary, đặt breakpoint, chạy từng lệnh, xem/sửa thanh ghi và bộ nhớ. **DIE (Detect It Easy)** là công cụ nhận diện: cho biết file là PE/ELF/Mach-O, được biên dịch bằng compiler nào (MinGW, MSVC, Delphi...), có bị pack hay không.

DIE trả lời câu hỏi đầu tiên của mọi ca phân tích: *"file này được tạo ra bằng gì?"* — từ đó chọn hướng phân tích đúng.

## Lab từng bước

1. Tải x64dbg snapshot, giải nén vào thư mục lab.
2. Tải DIE bản portable, chạy thử.
3. Nếu x64dbg báo lỗi thiếu `api-ms-win-crt-runtime-l1-1-0.dll` → cài VC++ Redistributable 2015–2022.
4. Mở DIE, kéo một file PE của bạn vào, đọc: file type, compiler, linker.
5. Chạy `python die_sim.py <file>` để đối chiếu signature mô phỏng.

## Liên kết code mẫu

Xem [code/week02/README.md](../code/week02/README.md) — script `die_sim.py` · `die_sim.cpp` (C++17 chi tiết)

## Thảo luận

- Vì sao biết compiler giúp chọn đúng cách phân tích?
- Sự khác nhau giữa debugger (x64dbg) và detector (DIE)?

## Bài tập

- **Cơ bản:** Cài x64dbg + DIE, chụp ảnh giao diện.
- **Nâng cao:** Dùng DIE phân tích 2 binary khác compiler, ghi lại khác biệt.
- **Thử thách:** Giải thích vì sao lỗi `api-ms-win-crt-runtime` xảy ra và cách sửa tổng quát.

## Rubric (100 điểm)

- Đúng chức năng (35): cài xong, DIE chạy và cho kết quả đúng.
- An toàn (25): chỉ dùng binary của bạn/crackme.
- Tài liệu (20): ảnh + mô tả rõ.
- Phân tích (20): giải thích ý nghĩa compiler/packer.

## An toàn & phạm vi

Công cụ tải từ nguồn chính thức. Không cài lên máy chính khi không cần; ưu tiên VM.
