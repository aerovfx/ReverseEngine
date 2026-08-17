---
layout: course
title: "Week01"
permalink: /agent-sandbox-10weeks/lessons/week01.html
---

# Tuần 01 — Vì sao agent cần sandbox & mô hình đe doạ

## Mục tiêu

- Giải thích rủi ro khi cho AI agent chạy tool/code trên máy thật.
- Xây dựng mô hình đe doạ cho một agent có quyền gọi tool.
- Nêu tổng quan CubeSandbox và dựng lab (đọc tài liệu, không cần KVM).

## Công cụ và dữ liệu

- Tài liệu CubeSandbox: [README](https://github.com/TencentCloud/CubeSandbox) + `docs/guide/introduction.md`.
- Script `code/week01/risk_model.py` (mô phỏng phân loại rủi ro tool).

## Lý thuyết

AI agent (ví dụ coding agent) cần chạy lệnh, cài package, đọc/ghi file, gọi mạng. Mỗi hành động là một **tool call** — nếu chạy trực tiếp trên máy chủ, một lệnh sai (hoặc tool độc hại) có thể: xoá dữ liệu, đọc secret, leo quyền, gọi C2.

**Sandbox** là môi trường cô lập cho phép agent "thoải mái" chạy tool mà không chạm hệ thống thật. Mô hình đe doạ cần trả lời ba câu hỏi:

1. **Ai** là tác nhân đe doạ? (agent bị prompt injection, tool độc hại, model ảo giác)
2. **Cái gì** cần bảo vệ? (dữ liệu chủ, secret, hạ tầng, người dùng khác)
3. **Bề mặt** tấn công? (process, syscall, mạng, filesystem, egress)

## Lab từng bước

1. Đọc README và introduction của CubeSandbox.
2. Chạy `python risk_model.py` để phân loại 5 tool theo mức rủi ro.
3. Viết mô hình đe doạ cho một coding agent (STRIDE đơn giản).
4. Ghi lại vì sao "cô lập" là yêu cầu bắt buộc chứ không phải tuỳ chọn.

## Liên kết code mẫu

Xem [code/week01/README.md](../code/week01/README.md) — `risk_model.cpp` (C++ chi tiết) và `risk_model.py` (Python mô phỏng nhanh).

## Thảo luận

- Prompt injection ảnh hưởng thế nào đến mô hình đe doạ của sandbox?
- Vì sao "chạy tool trên máy thật" luôn là rủi ro cao dù tool có vẻ vô hại?

## Bài tập

- **Cơ bản:** Liệt kê 5 rủi ro khi agent chạy tool không sandbox.
- **Nâng cao:** Xây mô hình đe doạ STRIDE cho một coding agent.
- **Thử thách:** Thiết kế chính sách "cho phép/chặn" cho 10 tool phổ biến.

## Rubric (100 điểm)

- Đúng chức năng (35): mô hình đe doạ đầy đủ, script chạy đúng.
- An toàn (25): nhận diện đúng rủi ro, không chạy tool thật trên máy chính.
- Tài liệu (20): bảng phân loại rõ ràng.
- Phân tích (20): giải thích được bề mặt tấn công.

## An toàn & phạm vi

Chỉ đọc tài liệu mã nguồn mở. Không chạy tool chưa rõ nguồn gốc trên máy thật.
