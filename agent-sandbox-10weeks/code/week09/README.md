---
layout: course
title: "Readme"
permalink: /agent-sandbox-10weeks/code/week09/README.html
---

# Tuần 09 — Code mẫu

## `lifecycle.cpp` (C++17, minh hoạ chi tiết)
State machine vòng đời sandbox + auto-pause.

```bash
g++ -std=c++17 -O2 lifecycle.cpp -o demo && ./demo
```

## `lifecycle_sim.py` (Python, mô phỏng nhanh)
Mô phỏng vòng đời đơn giản.

```bash
python lifecycle_sim.py
```

**An toàn:** cả hai đều thuần tính toán — không gọi syscall thật, không đụng filesystem/mạng, không chạy lệnh thật.
