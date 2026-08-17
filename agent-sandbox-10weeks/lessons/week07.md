---
layout: course
title: "Week07"
permalink: /agent-sandbox-10weeks/lessons/week07.html
---

# Tuần 07 — Egress zero-trust: L7 MITM proxy

## Mục tiêu

- Giải thích CubeEgress: L7 proxy chặn mọi HTTP/HTTPS ra ngoài.
- Phân tích domain allowlist, TLS inspection, credential injection, audit.
- Hiểu vì sao egress phải "zero-trust" (chỉ cho phép domain đã khai báo).

## Công cụ và dữ liệu

- `docs/guide/security-proxy.md`; `CubeEgress/` (OpenResty).
- Script `code/week07/egress_filter.py` (mô phỏng allowlist + injection).

## Lý thuyết

Sandbox không được phép "tự do" ra Internet — nếu không, agent bị prompt injection có thể gửi dữ liệu ra ngoài. **CubeEgress** ép toàn bộ lưu lượng ra ngoài đi qua một L7 proxy (TPROXY + OpenResty):

- **Domain allowlist** — cho phép/chặn theo SNI, host, method, scheme, path.
- **TLS inspection** — sandbox tin một root CA do CubeEgress cấp, nên proxy giải mã được HTTPS để kiểm tra.
- **Credential injection** — thêm `Authorization` header ở proxy, secret không bao giờ nằm trong sandbox.
- **Audit** — mọi quyết định ghi vào JSONL log.

Đây là tầng "zero-trust": mặc định từ chối, chỉ cho phép đúng domain đã khai báo.

## Lab từng bước

1. Đọc security-proxy.md.
2. Chạy `python egress_filter.py` để mô phỏng allowlist + injection.
3. Thiết kế policy cho phép agent gọi `api.openai.com` nhưng chặn mọi domain khác.
4. Giải thích vì sao credential injection tốt hơn "đưa secret vào sandbox".

## Liên kết code mẫu

Xem [code/week07/README.md](../code/week07/README.md) — script `egress_filter.py`.

## Thảo luận

- TLS inspection hoạt động thế nào nhờ root CA?
- Vì sao default-deny quan trọng hơn default-allow cho egress?

## Bài tập

- **Cơ bản:** Liệt kê 4 chức năng của CubeEgress.
- **Nâng cao:** Viết policy allowlist cho 3 domain, mô tả kết quả chặn.
- **Thử thách:** Giải thích credential injection giảm rủi ro rò rỉ secret thế nào.

## Rubric (100 điểm)

- Đúng chức năng (35): mô phỏng allowlist/injection đúng.
- An toàn (25): default-deny, secret không vào sandbox.
- Tài liệu (20): bảng policy rõ.
- Phân tích (20): giải thích zero-trust egress.

## An toàn & phạm vi

Chỉ mô phỏng bằng dữ liệu, không MITM lưu lượng người khác.
