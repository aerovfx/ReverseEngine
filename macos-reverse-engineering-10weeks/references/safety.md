---
layout: course
title: "Safety"
permalink: /macos-reverse-engineering-10weeks/references/safety.html
---

# An toàn và phạm vi được phép

## Nguyên tắc bắt buộc

- **RE ứng dụng thương mại chỉ trong phạm vi hợp pháp**: ứng dụng của chính bạn, ứng dụng được cấp phép cho mục đích nghiên cứu, hoặc ứng dụng mã nguồn mở.
- **Cấm** patch ứng dụng của bên thứ ba để dùng lậu, né DRM/license, hoặc phát tán bản crack — hành vi đó vi phạm bản quyền/DMCA.
- Mọi phân tích malware chạy trong **VM cô lập**, có snapshot, mạng tắt hoặc giả lập. Không bao giờ chạy mẫu trên máy chính.
- Không **phát tán** mẫu malware; không đưa mẫu lên nơi công cộng.
- Không đưa **mật khẩu giải nén mẫu**, token, khóa API hay dữ liệu cá nhân vào bài nộp/báo cáo.
- Không quét Internet, không thử mật khẩu, không né kiểm soát của tổ chức.

## Quy trình lab an toàn

1. Dựng VM macOS sạch, **snapshot "Base"**.
2. Chỉ vô hiệu SIP/AMFI **trong VM lab** (qua Recovery), không trên máy chính.
3. Cấu hình mạng host-only hoặc giả lập; không dùng NAT với Internet thật khi chạy mẫu.
4. Copy mẫu/app vào VM qua shared folder read-only.
5. Bật công cụ giám sát → **snapshot** → chạy → ghi nhận → **revert**.
6. Sau khi xong: xoá snapshot, xoá mẫu khỏi VM, dọn shared folder.

## Xử lý sự cố

- Nếu mẫu có dấu hiệu **thoát VM** hoặc kết nối mạng thật ngoài ý muốn: dừng VM ngay, ngắt mạng, bảo toàn bằng chứng, báo người phụ trách.
- Nếu phát hiện dữ liệu thật (không phải dữ liệu lab): dừng, không đọc thêm, báo cáo theo quy trình.

## Giới hạn của khoá học

Khoá học **không** dạy tấn công mục tiêu thật, không hướng dẫn khai thác hệ thống ngoài lab, không hướng dẫn crack app để lậu. Toàn bộ hoạt động phục vụ mục đích **phòng thủ, phân tích và hiểu biết**. Việc RE có thể bị ràng buộc bởi pháp luật địa phương; hãy đảm bảo bạn có quyền hợp pháp với mọi ứng dụng/mẫu và môi trường sử dụng.
