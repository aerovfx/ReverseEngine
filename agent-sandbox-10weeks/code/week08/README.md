---
layout: course
title: "Readme"
permalink: /agent-sandbox-10weeks/code/week08/README.html
---

# Tuần 08 — Code mẫu

## `tool_runner.cpp` (C++17, minh hoạ chi tiết)
Wrapper chạy tool an toàn: whitelist + denylist + giới hạn.

```bash
g++ -std=c++17 -O2 tool_runner.cpp -o demo && ./demo
```

## `tool_runner.py` (Python, mô phỏng nhanh)
Mô phỏng whitelist/denylist.

```bash
python tool_runner.py --cmd 'ls -la'
```

**An toàn:** cả hai đều thuần tính toán — không gọi syscall thật, không đụng filesystem/mạng, không chạy lệnh thật.
