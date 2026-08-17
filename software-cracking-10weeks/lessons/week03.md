---
layout: course
title: "Week03"
permalink: /software-cracking-10weeks/lessons/week03.html
---

# Tuần 03 — PE, base address & entry point

## Mục tiêu

- Giải thích ImageBase và AddressOfEntryPoint trong PE.
- Tính entry point: `EP = ImageBase + AddressOfEntryPoint`.
- Đọc header PE bằng script Python an toàn.

## Công cụ và dữ liệu

- Một file PE của bạn (hoặc crackme).
- Script `code/week03/pe_entry_point.py` (parse PE, không cần Windows).

## Lý thuyết

Khi Windows nạp một file PE vào bộ nhớ, nó đặt binary tại một **base address** (ImageBase, thường `0x00400000` cho 32-bit). Điểm bắt đầu thực thi là **entry point**, được lưu dưới dạng offset (RVA) trong header:

```
EntryPoint(VA) = ImageBase + AddressOfEntryPoint
```

Hiểu entry point là bước đầu để biết "bắt đầu đọc chương trình ở đâu" — mọi thao tác debug sau này đều xoay quanh điểm này.

## Lab từng bước

1. Chọn một file PE hợp pháp (app bạn tự biên dịch hoặc crackme).
2. Chạy `python pe_entry_point.py <file>` để đọc ImageBase và AddressOfEntryPoint.
3. Tính entry point và đối chiếu với DIE.
4. Mở x64dbg, nạp file, xác nhận debugger dừng ở entry point.
5. Ghi hash SHA-256 trước khi phân tích.

## Liên kết code mẫu

Xem [code/week03/README.md](../code/week03/README.md) — script `pe_entry_point.py` · `pe_entry_point.cpp` (C++17 chi tiết)

## Thảo luận

- Vì sao entry point là RVA chứ không phải địa chỉ tuyệt đối?
- ASLR ảnh hưởng thế nào đến base address?

## Bài tập

- **Cơ bản:** Đọc và ghi lại ImageBase + AddressOfEntryPoint của 1 file PE.
- **Nâng cao:** Tính entry point và xác nhận bằng x64dbg.
- **Thử thách:** Viết script đọc thêm số section và tên section của PE.

## Rubric (100 điểm)

- Đúng chức năng (35): script chạy, in đúng 2 trường.
- An toàn (25): chỉ dùng binary hợp pháp, có hash.
- Code (20): có chú thích, xử lý lỗi file không phải PE.
- Phân tích (20): giải thích được công thức EP.

## An toàn & phạm vi

Script chỉ đọc header, không thực thi binary. Không chạy mẫu lạ ngoài VM.
