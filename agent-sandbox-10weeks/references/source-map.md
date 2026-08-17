---
layout: course
title: "Source Map"
permalink: /agent-sandbox-10weeks/references/source-map.html
---

# Ánh xạ nguồn — CubeSandbox → 10 tuần

Nguồn: [TencentCloud/CubeSandbox](https://github.com/TencentCloud/CubeSandbox) (Apache 2.0).

| Tuần | Thành phần/thư mục repo | Tài liệu tham khảo |
|---:|---|---|
| 01 | Tổng quan, README, docs/guide/introduction.md | [Repo](https://github.com/TencentCloud/CubeSandbox) |
| 02 | `hypervisor/` (CubeHypervisor, RustVMM) | docs/architecture/overview.md |
| 03 | `CubeAPI/`, `CubeMaster/`, `Cubelet/` | docs/architecture/overview.md |
| 04 | `CubeShim/`, `hypervisor/` (snapshot/restore) | docs/guide/snapshot-rollback-clone.md |
| 05 | `cubecow/` (CubeCoW, FICLONE) | docs/guide/persistent-storage.md |
| 06 | `CubeNet/` (CubeVS, eBPF) | docs/architecture/network.md |
| 07 | `CubeEgress/` (OpenResty MITM) | docs/guide/security-proxy.md |
| 08 | `sdk/`, `examples/` (E2B) | docs/guide/quickstart.md |
| 09 | `CubeProxy/`, `cube-lifecycle-manager/`, `deploy/` | docs/guide/multi-node-deploy.md |
| 10 | Toàn repo | docs/guide/network-hardening.md |

## Lưu ý

- Khoá học chỉ phân tích mã nguồn mở (Apache 2.0), không sao chép mã vào bài học.
- Mọi đoạn mô tả kiến trúc được viết lại bằng tiếng Việt từ tài liệu công khai.
