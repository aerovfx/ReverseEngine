---
layout: course
title: "Readme"
permalink: /software-cracking-10weeks/code/week03/README.html
---

# Tuần 03 — Code mẫu

## `pe_entry_point.py`
Đọc ImageBase và AddressOfEntryPoint, tính entry point.

```bash
python pe_entry_point.py <file.exe>
```

**An toàn:** script chỉ mô phỏng/đọc dữ liệu, không thực thi binary, không sửa file gốc.
