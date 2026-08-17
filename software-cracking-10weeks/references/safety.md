---
layout: course
title: "Safety"
permalink: /software-cracking-10weeks/references/safety.html
---

# An toàn & pháp lý — Software Cracking

## Nguyên tắc bắt buộc

1. **Phạm vi hợp pháp.** Chỉ phân tích phần mềm bạn **sở hữu**, phần mềm **được ủy quyền bằng văn bản** cho mục đích nghiên cứu, hoặc **crackme** công khai (crackmes.one, các challenge tự tạo). Không nhắm vào phần mềm thương mại của người khác.
2. **Không phát tán crack/patch.** Mọi bản vá chỉ tồn tại trong lab của bạn để học. Cấm chia sẻ crack, keygen cho phần mềm thương mại, cấm né DRM/license để dùng lậu.
3. **Lab cô lập.** Chạy mẫu trong VM Windows riêng, tắt mạng hoặc dùng mạng giả lập, snapshot trước khi chạy.
4. **Chain of custody.** Ghi SHA-256 trước/sau khi phân tích, log từng bước, ẩn danh dữ liệu cá nhân trong báo cáo.

## Điều gì là hợp pháp

- Phân tích **crackme** — binary công khai được tạo ra để luyện RE.
- Phân tích **phần mềm của chính bạn** để tìm lỗi, hiểu cơ chế cấp phép.
- Báo cáo lỗ hổng có trách nhiệm (responsible disclosure).
- Nghiên cứu kỹ thuật chống vá (anti-tamper) để **bảo vệ** phần mềm của bạn.

## Điều gì vi phạm pháp luật

- Crack phần mềm thương mại để dùng lậu (vi phạm bản quyền, DMCA §1201, và luật tương đương).
- Phát tán bản crack, patch, keygen cho phần mềm người khác.
- Né DRM/license/đăng ký để chiếm dụng sản phẩm trả phí.

## Checklist trước khi thực hành

- [ ] Binary là của tôi / crackme / được phép bằng văn bản?
- [ ] VM cô lập đã snapshot?
- [ ] Đã ghi hash SHA-256 của binary?
- [ ] Báo cáo sẽ ẩn danh dữ liệu cá nhân?

> Nếu bất kỳ mục nào là "không", dừng lại và chọn mục tiêu hợp pháp khác.
