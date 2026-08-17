---
layout: course
title: "Final Project"
permalink: /software-cracking-10weeks/projects/final_project.html
---

# Dự án cuối khóa — Phân tích & bảo vệ một binary hợp pháp

## Bài toán

Chọn **một** trong hai hướng (đều phải hợp pháp):

- **(A) Phân tích crackme** từ crackmes.one (hoặc binary bạn tự viết). Tìm "flag"/serial ẩn bằng quy trình: DIE → entry point → string references → breakpoint → reversing jump/patch.
- **(B) Bảo vệ phần mềm của chính bạn.** Viết một app nhỏ có kiểm tra serial, rồi tự phân tích nó và đề xuất cách chống vá (anti-tamper).

## Phạm vi

- Chỉ binary của bạn / crackme công khai / được phép bằng văn bản.
- Không dùng phần mềm thương mại của người khác.
- Chạy trong VM cô lập, có snapshot.

## Yêu cầu chức năng

1. Báo cáo DIE: file type, compiler, packer (nếu có).
2. Tính và trình bày entry point (ImageBase + AddressOfEntryPoint).
3. Đặt ít nhất 1 breakpoint (trên chuỗi hoặc địa chỉ) và mô tả luồng.
4. Thực hiện 1 lần reversing jump hoặc patch (trên bản sao), kèm hash trước/sau.

## Yêu cầu phi chức năng

- An toàn: VM cô lập, chain of custody, ẩn danh dữ liệu.
- Tài liệu: mỗi bước có ảnh chụp hoặc log bằng chứng.
- Pháp lý: nêu rõ mục tiêu thuộc phạm vi nào (crackme / của bạn / được phép).

## Milestones

| Mốc | Nội dung | Tuần gợi ý |
|---|---|---|
| M1 | Chọn mục tiêu + RoE + hash | 8 |
| M2 | DIE + entry point + string references | 9 |
| M3 | Breakpoint + reversing jump/patch + báo cáo | 10 |

## Deliverables

- Báo cáo Markdown (hoặc PDF) đầy đủ các bước.
- Script Python hỗ trợ phân tích (hash, parse PE, mô phỏng).
- Hash SHA-256 trước/sau của binary.

## Rubric 100 điểm

| Tiêu chí | Điểm | Đạt | Không đạt |
|---|---|---|---|
| Đúng quy trình phân tích | 35 | DIE → EP → string → BP → patch đầy đủ, bằng chứng rõ | Thiếu bước, không có bằng chứng |
| An toàn & pháp lý | 25 | VM cô lập, RoE, hash, mục tiêu hợp pháp | Chạy ngoài VM, mục tiêu thương mại không phép |
| Chất lượng báo cáo | 20 | Cấu trúc rõ, ảnh/log, giải thích | Báo cáo sơ sài, thiếu giải thích |
| Phân tích & đề xuất phòng thủ | 20 | Giải thích luồng + đề xuất chống vá | Chỉ mô tả thao tác, không phân tích |

## Tiêu chí thất bại bắt buộc (fail)

- Dùng phần mềm thương mại của người khác để crack.
- Phát tán crack/patch/keygen.
- Không có bất kỳ bằng chứng chạy nào.
