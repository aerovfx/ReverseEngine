---
layout: course
title: "Readme"
permalink: /software-cracking-10weeks/code/README.html
---

# Code minh hoạ — Software Cracking

Mỗi tuần có 1 script Python mô phỏng **an toàn** một kỹ thuật trong quy trình cracking. Không script nào chạy binary thật — bạn có thể thực hành ngay trên máy bất kỳ.

| Tuần | Script | Mô phỏng |
|---|---|---|
| 01 | `week01/lab_checklist.py` | Checklist lab cô lập |
| 02 | `week02/die_sim.py` | Nhận diện file/compiler (DIE) |
| 03 | `week03/pe_entry_point.py` | Đọc PE, tính entry point |
| 04 | `week04/jump_sim.py` | CMP + ZF + nhảy điều kiện |
| 05 | `week05/step_sim.py` | Step over/into |
| 06 | `week06/breakpoint_sim.py` | Breakpoint phần mềm (INT3) |
| 07 | `week07/reverse_jump.py` | Đảo nhảy điều kiện |
| 08 | `week08/patcher.py` | Vá byte trên bản sao |
| 09 | `week09/winapi_sim.py` | Luồng GUI kiểm tra serial |
| 10 | `week10/serial_check.py` | Kiểm tra serial + keygen |

## Chạy

```bash
python week03/pe_entry_point.py <file.exe>   # ví dụ đọc PE
python week10/serial_check.py --keygen Alice
```

## An toàn

Tất cả script chỉ đọc/mô phỏng, không thực thi mẫu, không sửa file gốc (riêng `patcher.py` chỉ vá **bản sao**). Xem [WEEKLY_EXAMPLES.md](WEEKLY_EXAMPLES.md) để biết chi tiết từng tuần.
