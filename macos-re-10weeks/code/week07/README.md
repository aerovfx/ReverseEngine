# Tuần 07 — Code mẫu

## `launchagent_gen.py`
Sinh nội dung LaunchAgent plist mẫu (không ghi hệ thống).

```bash
python3 launchagent_gen.py com.example.app
```

## `plist_parse.py`
Parse plist XML đơn giản.

```bash
python3 plist_parse.py            # plist giả
python3 plist_parse.py <file>
```

**An toàn:** chỉ sinh/đọc; không tự đặt persistence trên máy thật.
