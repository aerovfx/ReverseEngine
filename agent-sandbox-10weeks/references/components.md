---
layout: course
title: "Components"
permalink: /agent-sandbox-10weeks/references/components.html
---

# Thành phần kiến trúc & thuật ngữ

## Control plane vs Data plane

| Lớp | Thành phần | Trách nhiệm |
|---|---|---|
| Control plane | CubeAPI, CubeMaster, WebUI, Redis | API gateway, lập lịch, điều phối trạng thái |
| Data plane | Cubelet, CubeShim, CubeHypervisor, CubeCoW, CubeVS, CubeEgress, CubeProxy | Vòng đời VM, storage, mạng, bảo mật, routing |

## Thuật ngữ

| Thuật ngữ | Nghĩa |
|---|---|
| MicroVM | Máy ảo tối giản, mỗi sandbox 1 kernel riêng |
| KVM | Kernel-based Virtual Machine — cô lập cấp phần cứng |
| seccomp | Giới hạn syscall của tiến trình/VMM |
| RustVMM | VMM viết bằng Rust (firecracker-like) |
| Snapshot/Restore | Lưu/phục hồi trạng thái bộ nhớ để boot nhanh |
| CoW (Copy-on-Write) | Chia sẻ extent, chỉ sao chép khi ghi |
| FICLONE | ioctl reflink của XFS để clone O(1) |
| eBPF | Chương trình chạy trong kernel, dùng cho mạng |
| LPM trie | Longest-prefix-match cho network policy |
| TPROXY | Chặn gói tin trong suốt để ép qua proxy |
| MITM | Man-in-the-middle — proxy giải mã TLS để kiểm soát |
| E2B | Giao thức/SDK sandbox chuẩn cho agent |

## Vòng đời sandbox

```
create → run → pause → resume → snapshot → clone → destroy
```

## Vòng đời request

```
Client/SDK → CubeAPI → CubeMaster → Cubelet → CubeShim → CubeHypervisor → MicroVM
```
