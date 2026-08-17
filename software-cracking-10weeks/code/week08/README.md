---
layout: course
title: "Readme"
permalink: /software-cracking-10weeks/code/week08/README.html
---

# Tuần 08 — Code mẫu

## `patcher.py`
Vá byte trên bản sao, kèm backup + hash trước/sau.

```bash
python patcher.py --file a.exe --offset 100 --bytes 9090
```

**An toàn:** script chỉ mô phỏng/đọc dữ liệu, không thực thi binary, không sửa file gốc.
