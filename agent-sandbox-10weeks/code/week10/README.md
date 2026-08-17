---
layout: course
title: "Readme"
permalink: /agent-sandbox-10weeks/code/week10/README.html
---

# Tuần 10 — Code mẫu

## `harden_check.cpp` (C++17, minh hoạ chi tiết)
Checklist hardening + syscall allowlist gợi ý.

```bash
g++ -std=c++17 -O2 harden_check.cpp -o demo && ./demo
```

## `harden_check.py` (Python, mô phỏng nhanh)
Checklist hardening dạng text.

```bash
python harden_check.py --passed 1,2,3
```

**An toàn:** cả hai đều thuần tính toán — không gọi syscall thật, không đụng filesystem/mạng, không chạy lệnh thật.
