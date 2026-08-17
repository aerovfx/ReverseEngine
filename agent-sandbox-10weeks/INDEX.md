---
layout: course
title: "Index"
permalink: /agent-sandbox-10weeks/INDEX.html
---

# Agent Sandbox Security — 10 tuần

Khoá học 10 tuần về **kỹ thuật tạo sandbox an toàn cho AI agent** và **cách test công cụ một cách an toàn**, phân tích kiến trúc thực chiến của [CubeSandbox](https://github.com/TencentCloud/CubeSandbox) — dịch vụ sandbox mã nguồn mở (Apache 2.0) của Tencent Cloud, xây trên **RustVMM + KVM**.

## Kết quả đầu ra

Sau 10 tuần, người học có thể:

- Giải thích **mô hình đe doạ** khi cho agent chạy tool/code, và vì sao cần cô lập.
- So sánh các mức cô lập: **container vs VM vs MicroVM (KVM)** và bề mặt escape.
- Mô tả kiến trúc CubeSandbox: control plane (CubeAPI/CubeMaster) vs data plane (Cubelet/CubeShim/CubeHypervisor).
- Giải thích kỹ thuật **khởi động nhanh** bằng snapshot/restore bộ nhớ (<100ms).
- Phân tích **storage Copy-on-Write** (FICLONE reflink), snapshot/clone O(1).
- Phân tích **mạng eBPF** (CubeVS) và **network policy** theo sandbox.
- Phân tích **egress zero-trust** (L7 MITM proxy, domain allowlist, TLS inspection, credential injection).
- **Test tool an toàn** trong sandbox bằng SDK (E2B-compatible): whitelist, timeout, giới hạn tài nguyên.
- Đánh giá **mô hình đe doạ của chính sandbox** và đề xuất hardening.

## Cấu trúc

- 10 bài học tăng dần từ lý thuyết cô lập đến triển khai và phân tích thực tế.
- 10 bộ code minh hoạ Python mô phỏng an toàn các kỹ thuật (không cần máy có KVM).
- Bài tập cơ bản, nâng cao, thử thách mỗi tuần.
- Rubric 100 điểm và dự án cuối khoá (phân tích/thiết kế một kịch bản sandbox).

## Nguồn tham khảo

- [CubeSandbox trên GitHub](https://github.com/TencentCloud/CubeSandbox) (Apache 2.0).
- Ánh xạ kiến trúc → tuần học: [references/source-map.md](references/source-map.md).

## Quy tắc an toàn và pháp lý (BẮT BUỘC)

> Chỉ dựng sandbox và test tool trên **hạ tầng bạn sở hữu hoặc được ủy quyền**. Không dùng kỹ thuật sandbox để **thoát khỏi** sandbox của người khác, không test tool độc hại trên hệ thống thật. Mọi thử nghiệm escape/security chạy trong môi trường lab cô lập. Xem [references/safety.md](references/safety.md).
