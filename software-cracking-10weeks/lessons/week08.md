---
layout: course
title: "Week08"
permalink: /software-cracking-10weeks/lessons/week08.html
---

# Tuần 08 — Patch binary & tổng kết quy trình

## Mục tiêu

- Vá (patch) lệnh bằng NOP hoặc sửa byte.
- Lưu bản vá vào file mới, so sánh hash trước/sau.
- Nắm quy trình hoàn chỉnh: DIE → EP → string → BP → reversing → patch.

## Công cụ và dữ liệu

- x64dbg (chức năng patch), crackme hoặc binary của bạn.
- Script `code/week08/patcher.py` (vá byte trên **bản sao** file, có backup + hash).

## Lý thuyết

**Patch** là sửa byte của binary để thay đổi hành vi vĩnh viễn:

- **NOP** (0x90 trên x86) — thay một lệnh bằng "không làm gì", ví dụ xoá lệnh nhảy.
- **Sửa byte** — đổi toán hạng của lệnh (ví dụ đổi opcode `JE`(0x74) thành `JNE`(0x75)).

Trong x64dbg: sửa lệnh → chuột phải → **Patch file** (hoặc Copy to executable → All modifications) → lưu thành file mới. Luôn vá trên **bản sao**, giữ binary gốc và ghi hash trước/sau.

## Lab từng bước

1. Backup binary, ghi SHA-256 bản gốc.
2. Nạp vào x64dbg, tới lệnh cần vá (tuần 07).
3. Sửa lệnh (NOP hoặc đổi opcode), kiểm tra hành vi.
4. Patch file → lưu bản vá mới, ghi SHA-256 bản vá.
5. Chạy bản vá, xác nhận hành vi đổi; so sánh hash.

## Liên kết code mẫu

Xem [code/week08/README.md](../code/week08/README.md) — script `patcher.py`.

## Thảo luận

- Vì sao luôn vá trên bản sao và ghi hash?
- NOP khác sửa opcode ở rủi ro nào?

## Bài tập

- **Cơ bản:** Vá 1 byte trên bản sao, ghi hash trước/sau.
- **Nâng cao:** Dùng NOP để xoá một lệnh nhảy, xác nhận hành vi.
- **Thử thách:** Viết script tự động tìm và vá một chuỗi byte cụ thể.

## Rubric (100 điểm)

- Đúng chức năng (35): vá thành công, bản vá chạy đúng.
- An toàn (25): backup + hash + bản sao.
- Tài liệu (20): hash trước/sau, các byte đã sửa.
- Phân tích (20): giải thích tác động của byte vá.

## An toàn & phạm vi

Chỉ vá binary của bạn/crackme. Không phát tán bản vá cho phần mềm thương mại.
