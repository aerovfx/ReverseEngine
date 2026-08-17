---
layout: course
title: "Components"
permalink: /software-cracking-10weeks/references/components.html
---

# Thành phần binary & thuật ngữ

## PE (Portable Executable)

Định dạng file thực thi Windows. Hai trường quan trọng khi debug:

- **ImageBase** — địa chỉ nền ưu tiên nạp vào bộ nhớ.
- **AddressOfEntryPoint (RVA)** — offset của điểm vào tương đối với ImageBase.

```
EntryPoint (VA) = ImageBase + AddressOfEntryPoint
```

## Thuật ngữ debugger

| Thuật ngữ | Nghĩa |
|---|---|
| Base address | Địa chỉ nền mà binary nạp vào bộ nhớ |
| Entry point (EP) | Điểm đầu tiên chạy khi nạp |
| Step over (F8) | Chạy qua 1 lệnh, không đi sâu vào call |
| Step into (F7) | Đi sâu vào bên trong call |
| Breakpoint (BP) | Điểm dừng; software BP = INT3, hardware BP dùng debug register |
| Run (F9) | Chạy tới breakpoint hoặc kết thúc |

## Jump & cờ

- `JMP` — nhảy không điều kiện.
- `JE/JNE/JZ/JNZ...` — nhảy có điều kiện, dựa trên cờ (ZF, CF...).
- `CMP a, b` — so sánh, đặt cờ; `TEST a, b` — kiểm tra bit.
- **Đảo nhảy** = đổi điều kiện (hoặc NOP) để luồng đi nhánh khác.

## Chuỗi & serial

Khi tìm serial/password: dùng **string references** (x64dbg: chuột phải → Search for → Current module → String references), đặt breakpoint trên chuỗi gợi ý ("try harder", "invalid", "correct"...), rồi lần ngược tới lệnh so sánh.
