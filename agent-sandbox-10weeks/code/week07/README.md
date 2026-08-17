---
layout: course
title: "Readme"
permalink: /agent-sandbox-10weeks/code/week07/README.html
---

# Tuần 07 — Code mẫu

## `egress.cpp` (C++17, minh hoạ chi tiết)
Egress zero-trust: domain allowlist + credential injection.

```bash
g++ -std=c++17 -O2 egress.cpp -o demo && ./demo
```

## `egress_filter.py` (Python, mô phỏng nhanh)
Mô phỏng allowlist + injection.

```bash
python egress_filter.py
```

**An toàn:** cả hai đều thuần tính toán — không gọi syscall thật, không đụng filesystem/mạng, không chạy lệnh thật.
