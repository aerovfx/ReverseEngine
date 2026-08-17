---
layout: course
title: "Readme"
permalink: /software-cracking-10weeks/code/week09/README.html
---

# Tuần 09 — Code mẫu

## `winapi_sim.py`
Mô phỏng luồng GUI: GetWindowText → so sánh → MessageBox.

```bash
python winapi_sim.py --serial RE-2024
```

**An toàn:** script chỉ mô phỏng/đọc dữ liệu, không thực thi binary, không sửa file gốc.
