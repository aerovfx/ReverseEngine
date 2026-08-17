---
layout: course
title: "Week06"
permalink: /agent-sandbox-10weeks/lessons/week06.html
---

# Tuần 06 — Mạng eBPF & network policy

## Mục tiêu

- Giải thích CubeVS dùng eBPF cho SNAT/DNAT, conntrack, policy.
- Phân tích network policy theo sandbox (LPM trie).
- Hiểu vì sao dùng eBPF thay vì iptables hàng loạt.

## Công cụ và dữ liệu

- `docs/architecture/network.md`; `CubeNet/` (CubeVS).
- Script `code/week06/network_policy.py` (mô phỏng policy allowlist/denylist).

## Lý thuyết

Mỗi sandbox có một TAP (giao diện mạng điểm-điểm) nối vào data plane. **CubeVS** (eBPF) gắn các chương trình BPF để:

- **SNAT/DNAT** mỗi sandbox — không cần hàng nghìn luật iptables.
- **Conntrack** có trạng thái (TCP 11 trạng thái, UDP, ICMP).
- **Network policy** dùng **LPM trie** (longest-prefix-match) — khớp prefix IP ở tốc độ đường truyền.

Network policy cho phép chỉ định sandbox nào được nói chuyện với đâu (CIDR/domain), đây là lớp phòng thủ trước khi ra egress.

## Lab từng bước

1. Đọc network.md (CubeVS Network Model).
2. Chạy `python network_policy.py` để mô phỏng khớp policy.
3. Thiết kế 3 luật policy cho một sandbox (ví dụ: chặn 0.0.0.0/0, cho phép 10.0.0.0/8).
4. Giải thích vì sao LPM trie khớp nhanh hơn duyệt danh sách.

## Liên kết code mẫu

Xem [code/week06/README.md](../code/week06/README.md) — script `network_policy.py`.

## Thảo luận

- eBPF mạnh hơn iptables ở điểm nào cho hàng nghìn sandbox?
- Network policy khác egress proxy (tuần 07) ở tầng nào?

## Bài tập

- **Cơ bản:** Giải thích SNAT/DNAT cho một sandbox.
- **Nâng cao:** Viết 3 luật policy và dự đoán kết quả khớp.
- **Thử thách:** Giải thích conntrack stateful giúp gì khi chặn kết nối.

## Rubric (100 điểm)

- Đúng chức năng (35): policy mô phỏng đúng.
- An toàn (25): chính sách default-deny.
- Tài liệu (20): bảng luật rõ.
- Phân tích (20): giải thích eBPF vs iptables.

## An toàn & phạm vi

Chỉ mô phỏng policy bằng dữ liệu, không gắn BPF lên máy người khác.
