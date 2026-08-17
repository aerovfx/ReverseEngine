---
layout: course
title: "Readme"
permalink: /software-cracking-10weeks/code/week06/README.html
---

# Tuần 06 — Code mẫu

## `breakpoint_sim.py`
Mô phỏng breakpoint phần mềm (INT3/0xCC).

```bash
python breakpoint_sim.py --bp 0040100A
```

**An toàn:** script chỉ mô phỏng/đọc dữ liệu, không thực thi binary, không sửa file gốc.
