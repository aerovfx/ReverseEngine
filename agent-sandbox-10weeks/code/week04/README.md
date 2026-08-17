---
layout: course
title: "Readme"
permalink: /agent-sandbox-10weeks/code/week04/README.html
---

# Tuần 04 — Code mẫu

## `snapshot.cpp` (C++17, minh hoạ chi tiết)
Snapshot/restore bộ nhớ + dirty-page tracking.

```bash
g++ -std=c++17 -O2 snapshot.cpp -o demo && ./demo
```

## `snapshot_sim.py` (Python, mô phỏng nhanh)
Mô phỏng snapshot/restore đơn giản.

```bash
python snapshot_sim.py
```

**An toàn:** cả hai đều thuần tính toán — không gọi syscall thật, không đụng filesystem/mạng, không chạy lệnh thật.
