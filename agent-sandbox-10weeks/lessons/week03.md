---
layout: course
title: "Week03"
permalink: /agent-sandbox-10weeks/lessons/week03.html
---

# Tuần 03 — Kiến trúc CubeSandbox: control plane vs data plane

## Mục tiêu

- Vẽ sơ đồ kiến trúc CubeSandbox (control/data plane).
- Mô tả vai trò CubeAPI, CubeMaster, Cubelet, CubeShim, CubeHypervisor.
- Theo dõi vòng đời một request `Sandbox.create()`.

## Công cụ và dữ liệu

- `docs/architecture/overview.md`; thư mục `CubeAPI/`, `CubeMaster/`, `Cubelet/`.
- Script `code/week03/architecture_sim.py` (mô phỏng vòng đời request).

## Lý thuyết

CubeSandbox tách **control plane** (điều phối, stateless) khỏi **data plane** (chạy VM, node-local):

- **CubeAPI** (Rust/Axum): REST gateway tương thích E2B, nhận `POST /sandboxes`.
- **CubeMaster** (Go): lập lịch — chọn node theo tài nguyên, gửi gRPC tới Cubelet.
- **Cubelet** (Go): quản lý vòng đời sandbox trên một node.
- **CubeShim** (Rust): triển khai containerd Shim v2, chuẩn bị rootfs/kernel, khởi động VM.
- **CubeHypervisor** (RustVMM + KVM): tạo microVM, vCPU, virtio device.

Luồng: `Client → CubeAPI → CubeMaster → Cubelet → CubeShim → CubeHypervisor → MicroVM`, trạng thái lưu ở Redis (control plane stateless).

## Lab từng bước

1. Đọc overview (Control Plane vs Data Plane, Core Components).
2. Chạy `python architecture_sim.py` để mô phỏng vòng đời request.
3. Vẽ sơ đồ kiến trúc (có thể dùng Mermaid trong báo cáo).
4. Giải thích vì sao control plane stateless giúp scale ngang.

## Liên kết code mẫu

Xem [code/week03/README.md](../code/week03/README.md) — script `architecture_sim.py`.

## Thảo luận

- Vì sao tách control plane khỏi data plane là quyết định kiến trúc quan trọng?
- Redis đóng vai trò gì khi control plane stateless?

## Bài tập

- **Cơ bản:** Kể tên 5 thành phần và một câu mô tả vai trò.
- **Nâng cao:** Vẽ sequence diagram của `Sandbox.create()`.
- **Thử thách:** Giải thích điều gì xảy ra nếu một CubeMaster instance bị crash.

## Rubric (100 điểm)

- Đúng chức năng (35): sơ đồ đúng, script chạy.
- An toàn (25): hiểu đúng trách nhiệm từng lớp.
- Tài liệu (20): sơ đồ rõ ràng.
- Phân tích (20): giải thích được stateless/scale.

## An toàn & phạm vi

Chỉ phân tích kiến trúc mã nguồn mở, không thay đổi hệ thống.
