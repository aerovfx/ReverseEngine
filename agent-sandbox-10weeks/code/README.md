---
layout: course
title: "Readme"
permalink: /agent-sandbox-10weeks/code/README.html
---

# Code minh hoạ — Agent Sandbox Security

Mỗi tuần có 1 script Python mô phỏng **an toàn** một kỹ thuật sandbox. Không script nào cần máy có KVM hay đụng hệ thống thật.

| Tuần | Script | Mô phỏng |
|---|---|---|
| 01 | `week01/risk_model.py` | Mô hình đe doạ tool |
| 02 | `week02/isolation_levels.py` | Các mức cô lập |
| 03 | `week03/architecture_sim.py` | Vòng đời request |
| 04 | `week04/snapshot_sim.py` | Snapshot/restore |
| 05 | `week05/cow_storage.py` | Copy-on-Write |
| 06 | `week06/network_policy.py` | Network policy |
| 07 | `week07/egress_filter.py` | Egress zero-trust |
| 08 | `week08/tool_runner.py` | Chạy tool an toàn |
| 09 | `week09/lifecycle_sim.py` | Vòng đời + auto-pause |
| 10 | `week10/harden_check.py` | Hardening checklist |

## Chạy

```bash
python week08/tool_runner.py --cmd "ls -la"
python week10/harden_check.py --passed 1,2,3
```

## An toàn

Tất cả script chỉ mô phỏng bằng dữ liệu, không chạy lệnh/tool thật. Xem [WEEKLY_EXAMPLES.md](WEEKLY_EXAMPLES.md).
