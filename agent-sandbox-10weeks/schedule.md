---
layout: course
title: "Schedule"
permalink: /agent-sandbox-10weeks/schedule.html
---

# Lộ trình Agent Sandbox Security — 10 tuần

| Tuần | Chủ đề | Thành phần CubeSandbox | Lab local | Sản phẩm nộp | Nối tiếp |
|---:|---|---|---|---|---|
| 01 | Vì sao agent cần sandbox & mô hình đe doạ | Tổng quan | Dựng lab + phân loại rủi ro tool | Bảng mô hình đe doạ | Nền tảng mọi tuần |
| 02 | Cô lập: container vs VM vs MicroVM | CubeHypervisor (KVM) | So sánh mức cô lập + seccomp | Bảng so sánh cô lập | Hiểu nền tảng KVM |
| 03 | Kiến trúc CubeSandbox | CubeAPI/CubeMaster/Cubelet | Vẽ sơ đồ control/data plane | Sơ đồ kiến trúc | Nắm bức tranh hệ thống |
| 04 | Khởi động nhanh & snapshot/restore | CubeShim/RustVMM | Mô phỏng snapshot bộ nhớ | Báo cáo snapshot | Kỹ thuật boot nhanh |
| 05 | Storage Copy-on-Write & clone O(1) | CubeCoW (FICLONE) | Mô phỏng reflink/clone | Báo cáo CoW | Hiểu snapshot/clone |
| 06 | Mạng eBPF & network policy | CubeVS | Mô phỏng policy LPM | Báo cáo network policy | Nền tảng egress |
| 07 | Egress zero-trust: L7 MITM proxy | CubeEgress | Mô phỏng domain allowlist + TLS | Báo cáo egress | Bảo mật chiều ra |
| 08 | Test tool an toàn trong sandbox | SDK (E2B) | Chạy tool với whitelist/timeout | Báo cáo test tool | Kỹ năng thực hành |
| 09 | Deploy & tích hợp | CubeProxy/lifecycle | Deploy single/multi-node + auto-pause | Mô tả deploy | Ứng dụng thực tế |
| 10 | Final project & hardening | Toàn hệ thống | Phân tích mô hình đe doạ + hardening | Final project | Lộ trình cá nhân |

## Cách đánh giá

Mỗi tuần dùng rubric 100 điểm: đúng chức năng 35, an toàn và xử lý lỗi 25, chất lượng code/tài liệu 20, phân tích và bằng chứng chạy 20.

## Quy tắc lab và pháp lý

- Chỉ dựng sandbox trên máy/hạ tầng bạn sở hữu hoặc được ủy quyền.
- Không test tool độc hại trên hệ thống thật; mọi thử nghiệm trong sandbox cô lập.
- Không dùng kiến thức để thoát sandbox của bên thứ ba.
- Giữ bằng chứng (log, hash) cho mọi bài phân tích.
