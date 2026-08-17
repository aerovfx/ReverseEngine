# Final Project — Báo cáo Reverse Engineering ứng dụng macOS

## Bài toán và phạm vi

Người học thực hiện **reverse engineering một ứng dụng macOS trong phạm vi hợp pháp** và viết **báo cáo RE hoàn chỉnh** theo khung `report_builder.py`. Báo cáo phải tổng hợp static, dynamic, runtime/hooking, network và persistence.

**Phạm vi cho phép (bắt buộc):** chỉ phân tích **ứng dụng của chính bạn**, ứng dụng **được cấp phép** cho mục đích nghiên cứu, hoặc ứng dụng **mã nguồn mở**. **Cấm:** patch ứng dụng của bên thứ ba để dùng lậu, né DRM/license, phát tán bản crack, phân tích malware ngoài VM cô lập, hoặc chạy mẫu trên máy chính.

## Yêu cầu

- **Chức năng:** báo cáo gồm — nhận dạng (hash, Mach-O, kiến trúc), static analysis (strings, symbol, Ghidra, entitlements), dynamic analysis (LLDB/Frida, hành vi file/network), runtime/hooking (swizzle/dylib/Frida nếu dùng), persistence (LaunchAgents), kết luận và phòng thủ.
- **Phi chức năng:** chỉ dùng app hợp pháp; không chứa secret/token/dữ liệu cá nhân; kết quả tái lập được; bằng chứng (log, ảnh, output script) đi kèm.
- **Milestone:** tuần 8 chốt mục tiêu + phạm vi hợp pháp; tuần 9 thực hiện phân tích + draft; tuần 10 hoàn thiện + demo.
- **Deliverables:** báo cáo RE (Markdown), bằng chứng (ảnh/log/output script), script tự viết nếu có, demo 5 phút.
- **Demo script:** giới thiệu mục tiêu → static → dynamic → runtime → kết luận → phòng thủ.

## Threat model / Risk assessment

Phân tích rủi ro của hoạt động: mẫu thoát VM (nếu phân tích malware), kết nối mạng thật ngoài ý muốn, vi phạm phạm vi hợp pháp, dữ liệu nhạy cảm trong báo cáo. Nêu tài sản, tác nhân, kiểm soát, residual risk và cách cleanup (xoá snapshot, xoá mẫu).

## Rubric 100 điểm

| Tiêu chí | Xuất sắc | Đạt | Cần cải thiện | Chưa đạt | Điểm |
|---|---|---|---|---|---:|
| Đúng chức năng | Đủ static+dynamic+runtime+IOC đầy đủ | Đủ luồng chính | Thiếu một phần | Không hoàn thành | 35 |
| An toàn và lỗi | Lab cô lập, phạm vi hợp pháp, cleanup đầy đủ | Có guardrail chính | Thiếu giới hạn | Vi phạm phạm vi | 25 |
| Code và tài liệu | Báo cáo rõ, có bằng chứng, tái lập được | Dễ đọc, đủ bằng chứng | Khó theo dõi | Không giải thích | 20 |
| Phân tích, bằng chứng | Lập luận và log thuyết phục | Có bằng chứng | Bằng chứng yếu | Không có | 20 |

**Tiêu chí thất bại bắt buộc:** phân tích/patch ứng dụng của bên thứ ba để dùng lậu, né DRM/license, phát tán bản crack, chạy malware trên máy thật hoặc kết nối mạng thật, chứa secret/dữ liệu cá nhân, hoặc báo cáo không thể tái lập.

## Chuẩn bàn giao CyberLearn

- **Milestone:** tuần 8 chốt mục tiêu + phạm vi hợp pháp; tuần 9 phân tích + draft; tuần 10 hoàn thiện, demo, retrospective.
- **Deliverables:** báo cáo RE, bằng chứng, script, demo 5 phút, đánh giá rủi ro/an toàn.
- **Tiêu chí thất bại bắt buộc:** vi phạm phạm vi hợp pháp; phát tán crack; chứa secret; không tái lập được.
