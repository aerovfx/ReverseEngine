---
layout: course
title: "Week10"
permalink: /agent-sandbox-10weeks/lessons/week10.html
---

# Tuần 10 — Final project & hardening

## Mục tiêu

- Tổng hợp toàn khoá: mô hình đe doạ, cô lập, storage, mạng, egress.
- Đánh giá và đề xuất hardening cho một sandbox.
- Hoàn thiện dự án cuối khóa.

## Công cụ và dữ liệu

- Toàn bộ tài liệu CubeSandbox; `docs/guide/network-hardening.md`.
- Script `code/week10/harden_check.py` (checklist đánh giá hardening).

## Lý thuyết

Hardening sandbox là giảm bề mặt tấn công ở mọi tầng:

1. **Cô lập** — microVM (KVM), seccomp cho VMM.
2. **Storage** — CoW, snapshot/rollback nhanh.
3. **Mạng** — policy default-deny, conntrack.
4. **Egress** — domain allowlist, TLS inspection, credential injection.
5. **Vận hành** — auto-pause, audit log, giới hạn tài nguyên.

Một checklist hardening tốt trả lời: *"nếu agent bị prompt injection, kẻ tấn công làm được gì tối đa?"* — và câu trả lời phải là "gần như không gì".

## Lab từng bước

1. Chạy `python harden_check.py` để đánh giá một sandbox (giả định/thực).
2. Đối chiếu với network-hardening.md của CubeSandbox.
3. Hoàn thiện final project theo [projects/final_project.md](../projects/final_project.md).
4. Trình bày: mô hình đe doạ → kiến trúc → cơ chế an toàn → bằng chứng → hardening.

## Liên kết code mẫu

Xem [code/week10/README.md](../code/week10/README.md) — script `harden_check.py`.

## Thảo luận

- Prompt injection là "mô hình đe doạ mặc định" của sandbox agent — đúng hay sai?
- Hardening nào cho hiệu quả cao nhất với chi phí thấp nhất?

## Bài tập

- **Cơ bản:** Liệt kê 5 tầng hardening của sandbox.
- **Nâng cao:** Đánh giá một sandbox bằng checklist và chỉ ra 2 điểm yếu.
- **Thử thách:** Đề xuất hardening cho egress (domain + secret + audit).

## Rubric (100 điểm)

- Đúng chức năng (35): checklist/đánh giá đầy đủ.
- An toàn (25): đề xuất đúng, không vượt phạm vi.
- Tài liệu (20): báo cáo rõ ràng.
- Phân tích (20): giải thích được rủi ro và biện pháp.

## An toàn & phạm vi

Chỉ đánh giá sandbox của bạn. Kết thúc khoá: bạn có khung phân tích sandbox agent hoàn chỉnh.
