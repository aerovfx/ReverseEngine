---
layout: course
title: "Readme"
permalink: /agent-sandbox-10weeks/code/week03/README.html
---

# Tuần 03 — Code mẫu

## `pipeline.cpp` (C++17, minh hoạ chi tiết)
Mô phỏng pipeline control/data plane bằng OOP.

```bash
g++ -std=c++17 -O2 pipeline.cpp -o demo && ./demo
```

## `architecture_sim.py` (Python, mô phỏng nhanh)
Vòng đời request dạng danh sách bước.

```bash
python architecture_sim.py
```

**An toàn:** cả hai đều thuần tính toán — không gọi syscall thật, không đụng filesystem/mạng, không chạy lệnh thật.
