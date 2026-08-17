---
layout: course
title: "Readme"
permalink: /agent-sandbox-10weeks/code/week02/README.html
---

# Tuần 02 — Code mẫu

## `isolation_levels.cpp` (C++17, minh hoạ chi tiết)
So sánh mức cô lập + syscall allowlist (mô phỏng seccomp).

```bash
g++ -std=c++17 -O2 isolation_levels.cpp -o demo && ./demo
```

## `isolation_levels.py` (Python, mô phỏng nhanh)
Bảng so sánh cô lập + syscall nguy hiểm.

```bash
python isolation_levels.py
```

**An toàn:** cả hai đều thuần tính toán — không gọi syscall thật, không đụng filesystem/mạng, không chạy lệnh thật.
