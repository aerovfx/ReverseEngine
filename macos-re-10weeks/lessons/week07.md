# Tuần 07 — Network & Persistence: LaunchAgents, Wireshark, Little Snitch

## Mục tiêu

- Phân tích hành vi mạng của app bằng **Wireshark** và **Little Snitch**.
- Hiểu cơ chế **persistence** trên macOS: LaunchAgents, LaunchDaemons, `~/Library`.
- Nhận diện dấu hiệu app/malware đặt persistence.

## Công cụ và dữ liệu

- Wireshark, Little Snitch (hoặc tường lửa hệ thống), `launchctl`.
- App mẫu (của bạn hoặc mã nguồn mở) có gọi mạng.

## Lý thuyết và ví dụ

**Network analysis** — quan sát app kết nối đâu, gửi gì. Dùng Wireshark bắt gói trên interface loopback, hoặc Little Snitch cảnh báo mỗi khi app kết nối.

**Persistence** — cách app tự chạy lại khi khởi động:
- `~/Library/LaunchAgents/*.plist` — chạy khi user đăng nhập.
- `/Library/LaunchDaemons/*.plist` — chạy ở mức hệ thống.
- Login items (`osascript`/`System Events`), cron, `at`.

Ví dụ đọc plist:
```bash
plutil -p ~/Library/LaunchAgents/com.example.app.plist
launchctl list | grep example
```

Khi RE app, cần kiểm tra app có tạo LaunchAgent không — đó là hành vi đáng chú ý (cả cho app hợp pháp lẫn malware).

## Lab từng bước

1. Viết app nhỏ gửi HTTP request (hoặc dùng app mã nguồn mở).
2. Chạy app trong VM, bắt bằng **Wireshark** trên loopback; ghi host/port.
3. Cài **Little Snitch** (hoặc dùng tường lửa) và quan sát cảnh báo kết nối.
4. Tạo một LaunchAgent mẫu cho app của bạn; kiểm tra bằng `launchctl list`.
5. Viết báo cáo network + persistence.

## Liên kết code mẫu

Xem [code/week07/README.md](../code/week07/README.md) — `launchagent_gen.py`, `plist_parse.py`.

## Thảo luận

- Vì sao persistence là dấu hiệu quan trọng khi phân tích app lạ?
- Little Snitch so với Wireshark khác nhau thế nào về mục đích dùng?

## Bài tập

- **Cơ bản:** Liệt kê các vị trí persistence trên macOS.
- **Nâng cao:** Tạo LaunchAgent cho app của bạn và kiểm tra qua `launchctl`.
- **Thử thách:** Viết script quét `~/Library/LaunchAgents` và tóm tắt mỗi plist.

## Nộp bài và rubric

Nộp báo cáo network/persistence + output Wireshark/launchctl. Rubric chuẩn 100 điểm.

> **Cảnh báo pháp lý:** chỉ phân tích mạng/persistence của app của chính bạn hoặc app được cấp phép trong VM. Không dùng kỹ thuật để theo dõi app của người khác trái phép. Tuần 08 chuyển sang RE malware macOS.
