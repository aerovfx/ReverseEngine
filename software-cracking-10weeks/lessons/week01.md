---
layout: course
title: "Week01"
permalink: /software-cracking-10weeks/lessons/week01.html
---

# Tuần 01 — Tổng quan Software Ethical Hacking & môi trường lab

## Mục tiêu

- Phân biệt reverse engineering, software cracking và ranh giới đạo đức/pháp lý.
- Nêu được quy trình 8 bước của khóa (từ triage tới patch).
- Dựng môi trường lab Windows cô lập và viết Rules of Engagement (RoE).

## Công cụ và dữ liệu

- VM Windows (VirtualBox/VMware/Hyper-V), snapshot.
- Python 3 để chạy script minh hoạ an toàn.
- Script `code/week01/lab_checklist.py` (sinh checklist lab).

## Lý thuyết

**Software cracking** trong ngữ cảnh hợp pháp là dùng kỹ thuật reverse engineering để **hiểu** cơ chế kiểm tra của một chương trình (serial, license, thông báo) — không phải để dùng lậu. Ba phạm vi hợp pháp:

1. Phần mềm **bạn sở hữu** (tự viết).
2. Phần mềm **được ủy quyền bằng văn bản** cho nghiên cứu.
3. **Crackme** công khai (crackmes.one) — binary được tạo ra để luyện tập.

Quy trình làm việc xuyên suốt khóa:

```
Triage (DIE) → Entry point → Step → Breakpoint → Reversing jump → Patch → Báo cáo
```

## Lab từng bước

1. Tạo VM Windows, cài snapshot "Base".
2. Cài Python 3 trên VM.
3. Tạo file `roe.md` ghi: mục tiêu phân tích, phạm vi, công cụ, phương án rollback.
4. Chạy `python lab_checklist.py` để sinh checklist cô lập.
5. Ghi hash của mọi binary trước khi phân tích.

## Liên kết code mẫu

Xem [code/week01/README.md](../code/week01/README.md) — script `lab_checklist.py` · `lab_checklist.cpp` (C++17 chi tiết)

## Thảo luận

- Vì sao "crack để học" khác "crack để dùng lậu" về mặt pháp lý?
- Điều gì xảy ra nếu bạn phân tích phần mềm thương mại của người khác?

## Bài tập

- **Cơ bản:** Liệt kê 3 phạm vi hợp pháp và 3 hành vi bị cấm.
- **Nâng cao:** Viết RoE cho một crackme cụ thể, nêu phương án rollback.
- **Thử thách:** Thiết kế checklist cô lập 10 mục cho lab cracking, kèm lý do từng mục.

## Rubric (100 điểm)

- Đúng chức năng (35): checklist/RoE đầy đủ, chạy được.
- An toàn & pháp lý (25): nêu đúng phạm vi, không có mục tiêu thương mại.
- Chất lượng tài liệu (20): rõ ràng, có cấu trúc.
- Phân tích (20): giải thích được vì sao cần từng mục.

## An toàn & phạm vi

Chỉ thực hành trên hệ thống bạn sở hữu. Không tải/crack phần mềm thương mại. Xem [references/safety.md](../references/safety.md).
