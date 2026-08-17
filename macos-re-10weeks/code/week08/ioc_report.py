#!/usr/bin/env python3
# ============================================================
# ioc_report.py — Sinh khung báo cáo IOC (Indicators of Compromise)
# Mục tiêu: tạo template báo cáo IOC để điền kết quả phân tích.
# Đầu vào: không.
# Đầu ra: khung báo cáo IOC (in ra).
# An toàn: dữ liệu do bạn điền; không chứa secret/mẫu thật.
# ============================================================

def main():
    print("""# Báo cáo IOC — macOS

## 1. Nhận dạng mẫu
- Hash (SHA-256): <hash>
- Kiến trúc / định dạng: <arm64 / Mach-O>
- Tên file: <name>

## 2. Static analysis
- Strings đáng chú ý: <URL, key, path>
- Symbol / class: <class-dump, nm>
- Entitlements / code signing: <codesign output>

## 3. Dynamic analysis
- File ops: <fs_usage>
- Process: <tiến trình tạo ra>
- Network: <host, port>

## 4. Persistence
- LaunchAgents: <plist path>
- Login items / cron: <nếu có>

## 5. Indicators of Compromise
| Loại | Giá trị |
|---|---|
| File | <path, hash> |
| Network | <domain, IP, port> |
| Plist | <label, path> |

## 6. Kết luận và phòng thủ
<nhận định + biện pháp>
""")


if __name__ == "__main__":
    main()
