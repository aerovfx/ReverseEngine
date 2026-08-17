---
layout: course
title: "Readme"
permalink: /agent-sandbox-10weeks/code/README.html
---

# Code minh hoạ — Agent Sandbox Security

Mỗi tuần có **một chương trình C++17 chi tiết** (biên dịch bằng `g++`) minh hoạ kỹ thuật sandbox, kèm một script Python mô phỏng nhanh. Tất cả đều **thuần tính toán** — không cần máy có KVM, không gọi syscall/đụng filesystem/mạng thật.

| Tuần | C++ (chi tiết) | Python (mô phỏng nhanh) |
|---|---|---|
| 01 | `week01/risk_model.cpp` | `week01/risk_model.py` |
| 02 | `week02/isolation_levels.cpp` | `week02/isolation_levels.py` |
| 03 | `week03/pipeline.cpp` | `week03/architecture_sim.py` |
| 04 | `week04/snapshot.cpp` | `week04/snapshot_sim.py` |
| 05 | `week05/cow.cpp` | `week05/cow_storage.py` |
| 06 | `week06/policy.cpp` | `week06/network_policy.py` |
| 07 | `week07/egress.cpp` | `week07/egress_filter.py` |
| 08 | `week08/tool_runner.cpp` | `week08/tool_runner.py` |
| 09 | `week09/lifecycle.cpp` | `week09/lifecycle_sim.py` |
| 10 | `week10/harden_check.cpp` | `week10/harden_check.py` |

## Chạy

```bash
# C++ (chi tiết)
g++ -std=c++17 -O2 week08/tool_runner.cpp -o tool_runner && ./tool_runner

# Python (mô phỏng nhanh)
python week08/tool_runner.py --cmd "ls -la"
```

## An toàn

Mọi ví dụ đều thuần tính toán, không chạy lệnh/tool thật, không thao tác hệ thống. Xem [WEEKLY_EXAMPLES.md](WEEKLY_EXAMPLES.md).
