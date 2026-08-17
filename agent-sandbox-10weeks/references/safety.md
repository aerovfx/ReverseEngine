---
layout: course
title: "Safety"
permalink: /agent-sandbox-10weeks/references/safety.html
---

# An toàn & pháp lý — Agent Sandbox

## Nguyên tắc bắt buộc

1. **Sở hữu / ủy quyền.** Chỉ dựng sandbox và chạy tool trên hạ tầng bạn sở hữu hoặc được ủy quyền bằng văn bản.
2. **Không thoát sandbox của người khác.** Kỹ thuật phân tích cô lập không được dùng để trốn thoát khỏi sandbox, container, VM của bên thứ ba.
3. **Test an toàn.** Tool chưa rõ nguồn gốc chỉ chạy trong sandbox cô lập, có snapshot, hạn chế mạng.
4. **Không chạy malware trên máy thật.** Mẫu độc hại luôn chạy trong VM/microVM cô lập.
5. **Bằng chứng.** Ghi log, hash, và scope cho mọi bài phân tích.

## Điều gì là hợp pháp

- Phân tích mã nguồn mở (CubeSandbox — Apache 2.0) để học kiến trúc.
- Dựng sandbox của riêng bạn, test tool của bạn.
- Đánh giá bảo mật sandbox của chính bạn, báo cáo lỗ hổng có trách nhiệm.

## Điều gì vi phạm

- Dùng kỹ thuật sandbox để tấn công/thoát khỏi hạ tầng người khác.
- Chạy/phát tán malware, tool khai thác lỗ hổng.
- Né kiểm soát bảo mật của dịch vụ bên thứ ba.

## Checklist trước khi thực hành

- [ ] Hạ tầng là của tôi / được phép?
- [ ] Sandbox có snapshot/rollback?
- [ ] Mạng egress bị hạn chế (allowlist)?
- [ ] Tool chạy với whitelist + timeout + giới hạn tài nguyên?

> Nếu bất kỳ mục nào là "không", dừng lại và chọn môi trường hợp pháp khác.
