# Tuần 09 — Code mẫu

## `license_check.c`
App kiểm tra license (app của CHÍNH BẠN) để học RE.

```bash
clang license_check.c -o /tmp/license
/tmp/license SECRETKEY123
```

## `frida_license_hook.js`
Hook hàm `check_license` bằng Frida (chỉ trên app của bạn).

```bash
frida -n license -l frida_license_hook.js
```

> **Cảnh báo pháp lý:** CHỈ dùng trên app do chính bạn viết. CẤM patch app của bên thứ ba để dùng lậu (vi phạm bản quyền/DMCA).
