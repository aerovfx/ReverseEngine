---
layout: course
title: "Readme"
permalink: /agent-sandbox-10weeks/code/week01/README.html
---

# Tuần 01 — Code mẫu

## `risk_model.cpp` (C++17, minh hoạ chi tiết)
Phân loại rủi ro theo năng lực (capability) của tool.

```bash
g++ -std=c++17 -O2 risk_model.cpp -o demo && ./demo
```

## `risk_model.py` (Python, mô phỏng nhanh)
Mô phỏng nhanh mô hình đe doạ.

```bash
python risk_model.py
```

**An toàn:** cả hai đều thuần tính toán — không gọi syscall thật, không đụng filesystem/mạng, không chạy lệnh thật.
