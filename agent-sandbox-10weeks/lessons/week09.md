---
layout: course
title: "Week09"
permalink: /agent-sandbox-10weeks/lessons/week09.html
---

# Tuần 09 — Deploy & tích hợp

## Mục tiêu

- Phân biệt single-node, multi-node, K8s, Terraform deploy.
- Giải thích CubeProxy routing và auto-pause/resume.
- Lập kế hoạch tích hợp sandbox vào ứng dụng agent.

## Công cụ và dữ liệu

- `docs/guide/multi-node-deploy.md`, `docs/guide/security-proxy.md`; `deploy/`.
- Script `code/week09/lifecycle_sim.py` (mô phỏng vòng đời + auto-pause).

## Lý thuyết

- **Single-node** — mọi thành phần trên một máy (cần KVM hoặc PVM).
- **Multi-node** — control plane tách khỏi các compute node (Cubelet), scale ngang.
- **K8s/Terraform** — triển khai control plane + compute node dưới dạng cluster.

**CubeProxy** (OpenResty) route request tới sandbox theo host hoặc path. **cube-lifecycle-manager** theo dõi sự kiện để **auto-pause** sandbox rảnh và **resume** khi có request — tiết kiệm tài nguyên cho hàng nghìn sandbox.

## Lab từng bước

1. Đọc multi-node-deploy và security-proxy.
2. Chạy `python lifecycle_sim.py` để mô phỏng create/run/pause/resume.
3. Vẽ sơ đồ deploy multi-node (control plane + N compute node).
4. Giải thích auto-pause giúp tiết kiệm tài nguyên thế nào.

## Liên kết code mẫu

Xem [code/week09/README.md](../code/week09/README.md) — `lifecycle.cpp` (C++ chi tiết) và `lifecycle_sim.py` (Python mô phỏng nhanh).

## Thảo luận

- Vì sao control plane stateless (Redis) giúp multi-node dễ scale?
- Auto-pause/resume khác gì destroy/create lại?

## Bài tập

- **Cơ bản:** So sánh single-node và multi-node.
- **Nâng cao:** Vẽ topo deploy multi-node.
- **Thử thách:** Thiết kế chiến lược auto-pause cho 1000 sandbox rảnh.

## Rubric (100 điểm)

- Đúng chức năng (35): mô phỏng lifecycle đúng.
- An toàn (25): hiểu đúng routing/security.
- Tài liệu (20): sơ đồ deploy rõ.
- Phân tích (20): giải thích scale/auto-pause.

## An toàn & phạm vi

Chỉ lập kế hoạch deploy trên hạ tầng của bạn. Không triển khai lên máy người khác.
