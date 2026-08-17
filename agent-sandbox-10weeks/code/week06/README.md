---
layout: course
title: "Readme"
permalink: /agent-sandbox-10weeks/code/week06/README.html
---

# Tuần 06 — Code mẫu

## `policy.cpp` (C++17, minh hoạ chi tiết)
Network policy CIDR longest-prefix-match, default-deny.

```bash
g++ -std=c++17 -O2 policy.cpp -o demo && ./demo
```

## `network_policy.py` (Python, mô phỏng nhanh)
Mô phỏng khớp policy theo CIDR.

```bash
python network_policy.py
```

**An toàn:** cả hai đều thuần tính toán — không gọi syscall thật, không đụng filesystem/mạng, không chạy lệnh thật.
