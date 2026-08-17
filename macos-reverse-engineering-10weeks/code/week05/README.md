---
layout: course
title: "Readme"
permalink: /macos-reverse-engineering-10weeks/code/week05/README.html
---

# Tuần 05 — Code mẫu

## `frida_hook_template.js`
Template hook hàm bằng Frida (ví dụ `strcmp`).

```bash
frida -n <YourApp> -l frida_hook_template.js
```

## `dynamic_log_parser.py`
Phân tích log dynamic giả (file/network/process).

```bash
python3 dynamic_log_parser.py            # log giả
python3 dynamic_log_parser.py <file>
```

**An toàn:** chỉ hook app của chính bạn / được cấp phép.
