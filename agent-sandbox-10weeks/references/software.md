---
layout: course
title: "Software"
permalink: /agent-sandbox-10weeks/references/software.html
---

# Công cụ & phần mềm

| Công cụ | Vai trò | Ghi chú |
|---|---|---|
| **CubeSandbox** | Sandbox cho AI agent (RustVMM + KVM) | Mã nguồn mở Apache 2.0, E2B-compatible |
| **RustVMM** | Virtual Machine Monitor (VMM) | Tạo/quản lý microVM, snapshot/restore |
| **KVM** | Ảo hoá phần cứng Linux | Cô lập cấp phần cứng |
| **eBPF** | Lập trình kernel cho mạng | CubeVS: SNAT/DNAT, policy, tracking |
| **E2B SDK** | SDK client (Python/JS) | `Sandbox.create()`, chạy code, file, process |
| **OpenResty (nginx+Lua)** | L7 proxy | CubeProxy (routing), CubeEgress (MITM) |
| **XFS + FICLONE** | Filesystem reflink | Copy-on-Write, snapshot/clone O(1) |
| **Redis** | Lưu trạng thái control plane | Single source of truth |
| **containerd** | Runtime container | CubeShim triển khai Shim v2 |

## Cài đặt nhanh (tham khảo)

```bash
# Yêu cầu: Linux x86_64/aarch64, KVM (bare-metal) hoặc PVM, XFS tại /data/cubelet
# Xem docs/guide/quickstart.md của CubeSandbox để cài chính xác
curl -fsSL https://get.cubesandbox.io | bash   # (minh hoạ — dùng script chính thức của repo)
```

## Lưu ý

- Cần glibc ≥ 2.31 và filesystem XFS cho CoW.
- `/data/cubelet` tối thiểu 50 GB.
