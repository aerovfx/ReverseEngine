---
layout: course
title: "Readme"
permalink: /agent-sandbox-10weeks/code/week05/README.html
---

# Tuần 05 — Code mẫu

## `cow.cpp` (C++17, minh hoạ chi tiết)
Copy-on-Write với refcount (chia sẻ extent + tách khi ghi).

```bash
g++ -std=c++17 -O2 cow.cpp -o demo && ./demo
```

## `cow_storage.py` (Python, mô phỏng nhanh)
Mô phỏng shared extent + CoW.

```bash
python cow_storage.py
```

**An toàn:** cả hai đều thuần tính toán — không gọi syscall thật, không đụng filesystem/mạng, không chạy lệnh thật.
