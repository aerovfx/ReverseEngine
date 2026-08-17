---
layout: course
title: "Final Project"
permalink: /agent-sandbox-10weeks/projects/final_project.html
---

# Dự án cuối khóa — Phân tích & thiết kế sandbox an toàn

## Bài toán

Chọn **một** trong hai hướng:

- **(A) Phân tích kiến trúc.** Đọc mã nguồn CubeSandbox (một thành phần, ví dụ CubeEgress hoặc CubeCoW), viết báo cáo: mục đích, luồng dữ liệu, cơ chế bảo mật, và một điểm cần hardening.
- **(B) Thiết kế mini sandbox.** Dùng container + seccomp + network namespace (hoặc firecracker/microVM nếu có KVM) để dựng một sandbox tối giản chạy tool của agent, kèm whitelist + timeout + giới hạn tài nguyên.

## Phạm vi

- Chỉ hạ tầng của bạn, mã nguồn mở, hoặc được ủy quyền.
- Không tấn công/thoát sandbox của bên thứ ba.

## Yêu cầu chức năng

1. Mô hình đe doạ: liệt kê ít nhất 3 rủi ro khi agent chạy tool.
2. Mô tả kiến trúc (control/data plane) hoặc sandbox bạn dựng.
3. Chứng minh một cơ chế an toàn hoạt động (allowlist/timeout/cô lập mạng).
4. Báo cáo bằng chứng (log, ảnh, hash).

## Yêu cầu phi chức năng

- An toàn: sandbox cô lập, mạng hạn chế, có rollback.
- Tài liệu: cấu trúc rõ, có sơ đồ.
- Pháp lý: nêu rõ phạm vi được phép.

## Milestones

| Mốc | Nội dung | Tuần gợi ý |
|---|---|---|
| M1 | Chọn hướng + mô hình đe doạ | 8 |
| M2 | Dựng/phân tích kiến trúc + cơ chế an toàn | 9 |
| M3 | Báo cáo + demo + hardening | 10 |

## Deliverables

- Báo cáo Markdown/PDF.
- Script/cấu hình sandbox (nếu hướng B).
- Bằng chứng chạy.

## Rubric 100 điểm

| Tiêu chí | Điểm | Đạt | Không đạt |
|---|---|---|---|
| Đúng chức năng | 35 | Cơ chế an toàn chạy đúng, bằng chứng rõ | Thiếu cơ chế, không bằng chứng |
| An toàn & pháp lý | 25 | Lab cô lập, phạm vi hợp pháp | Test trên hệ thống thật, vượt phạm vi |
| Chất lượng báo cáo | 20 | Sơ đồ rõ, log/ảnh, giải thích | Sơ sài, thiếu giải thích |
| Phân tích & hardening | 20 | Phân tích đe doạ + đề xuất hardening | Chỉ mô tả, không phân tích |

## Tiêu chí thất bại bắt buộc (fail)

- Tấn công/thoát sandbox của người khác.
- Chạy malware/tool khai thác trên máy thật.
- Không có bằng chứng chạy nào.
