---
layout: course
title: "Week02"
permalink: /agent-sandbox-10weeks/lessons/week02.html
---

# Tuần 02 — Cô lập: container vs VM vs MicroVM

## Mục tiêu

- So sánh ba mức cô lập: container, máy ảo (VM), microVM (KVM).
- Giải thích bề mặt escape và vai trò của seccomp.
- Hiểu vì sao CubeSandbox chọn KVM (cô lập cấp phần cứng).

## Công cụ và dữ liệu

- Tài liệu `docs/architecture/overview.md`; thư mục `hypervisor/` (RustVMM).
- Script `code/week02/isolation_levels.py` (so sánh mức cô lập + syscall).

## Lý thuyết

| Mức | Chia sẻ kernel? | Overhead | Escape surface |
|---|---|---|---|
| Container (Docker) | Có (cùng kernel) | Thấp | Lớn — syscall, kernel vuln |
| VM (QEMU/KVM) | Không (kernel riêng) | Cao | Nhỏ — hypervisor |
| MicroVM (firecracker/RustVMM) | Không | Rất thấp | Nhỏ — VMM tối giản |

**Container** nhanh nhưng cùng kernel → một lỗ hổng kernel là thoát. **VM** cô lập tốt nhưng nặng. **MicroVM** là điểm cân bằng: kernel riêng + VMM tối giản (vài chục MB, khởi động ms), chính là cách CubeSandbox đạt "hardware-level isolation" với <5MB overhead.

**seccomp** giới hạn syscall mà tiến trình (kể cả VMM) được gọi, thu hẹp bề mặt tấn công.

## Lab từng bước

1. Đọc overview (phần CubeHypervisor, design principles).
2. Chạy `python isolation_levels.py` để so sánh mức cô lập.
3. Liệt kê 5 syscall nguy hiểm mà sandbox nên chặn (ví dụ `ptrace`, `mount`, `kexec`).
4. Giải thích vì sao "cùng kernel" là rủi ro lớn nhất của container.

## Liên kết code mẫu

Xem [code/week02/README.md](../code/week02/README.md) — script `isolation_levels.py`.

## Thảo luận

- MicroVM khác VM truyền thống ở điểm nào khiến overhead thấp hơn nhiều?
- Nếu chỉ cần chạy script ngắn, container có đủ an toàn không? Khi nào thì không?

## Bài tập

- **Cơ bản:** Lập bảng so sánh 3 mức cô lập (tốc độ, overhead, escape).
- **Nâng cao:** Giải thích seccomp giảm bề mặt tấn công thế nào.
- **Thử thách:** Thiết kế chính sách seccomp cho một sandbox chạy Python.

## Rubric (100 điểm)

- Đúng chức năng (35): bảng so sánh đúng, script chạy.
- An toàn (25): hiểu đúng escape surface.
- Tài liệu (20): bảng rõ ràng.
- Phân tích (20): giải thích được trade-off tốc độ/cô lập.

## An toàn & phạm vi

Chỉ phân tích mã nguồn mở. Không thử escape trên hệ thống người khác.
