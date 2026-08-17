---
layout: course
title: "Weekly Examples"
permalink: /software-cracking-10weeks/code/WEEKLY_EXAMPLES.html
---

# Danh sách ví dụ theo tuần

## Tuần 01 — lab_checklist.py
Sinh checklist lab cô lập. `python week01/lab_checklist.py --out checklist.txt`

## Tuần 02 — die_sim.py
Đọc magic bytes, đoán định dạng/compiler. `python week02/die_sim.py <file>`

## Tuần 03 — pe_entry_point.py
Đọc ImageBase + AddressOfEntryPoint. `python week03/pe_entry_point.py <file.exe>`

## Tuần 04 — jump_sim.py
Mô phỏng CMP → ZF → JE/JNE. `python week04/jump_sim.py`

## Tuần 05 — step_sim.py
Mô phỏng step over (F8) vs step into (F7). `python week05/step_sim.py`

## Tuần 06 — breakpoint_sim.py
Mô phỏng BP phần mềm (INT3). `python week06/breakpoint_sim.py --bp 0040100A`

## Tuần 07 — reverse_jump.py
Mô phỏng đảo nhảy. `python week07/reverse_jump.py`

## Tuần 08 — patcher.py
Vá byte trên bản sao + hash. `python week08/patcher.py --file a.exe --offset 100 --bytes 9090`

## Tuần 09 — winapi_sim.py
Mô phỏng luồng GUI. `python week09/winapi_sim.py --serial RE-2024`

## Tuần 10 — serial_check.py
Kiểm tra/sinh serial. `python week10/serial_check.py --keygen Alice`
