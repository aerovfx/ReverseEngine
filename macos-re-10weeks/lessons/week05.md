# Tuần 05 — Dynamic Analysis: LLDB, Frida, fs_usage, unified logging

## Mục tiêu

- Dùng **LLDB** để đặt breakpoint, đọc thanh ghi, bước qua hàm.
- Dùng **Frida** để hook hàm và đọc tham số runtime.
- Theo dõi hành vi file/mạng bằng **fs_usage** và **unified logging**.

## Công cụ và dữ liệu

- LLDB (Xcode CLT), Frida (`pip install frida-tools`).
- `fs_usage`, `log stream`, `sample`.
- App mẫu do chính bạn viết (để hook hợp pháp).

## Lý thuyết và ví dụ

**Dynamic analysis** là quan sát hành vi khi chạy. Trên macOS:

- **LLDB** — debugger: `breakpoint set`, `run`, `register read`, `step`, `x/s` (đọc chuỗi tại địa chỉ).
- **Frida** — hook runtime: `frida -n AppName` rồi `Interceptor.attach` để chặn hàm, đọc/ghi tham số.
- **fs_usage** — theo dõi syscall file; **log stream** — đọc unified logging của app.

Ví dụ LLDB:
```bash
lldb ./hello
(lldb) breakpoint set --name main
(lldb) run
(lldb) register read x0
(lldb) x/s $x0
```

Ví dụ Frida (JS):
```js
Interceptor.attach(Module.findExportByName(null, "strcmp"), {
  onEnter(args) { console.log("strcmp", args[0].readCString(), args[1].readCString()); }
});
```

## Lab từng bước

1. Build app mẫu (C hoặc Objective-C) có hàm nhận input.
2. Chạy app trong **LLDB**, đặt breakpoint ở `main` và hàm xử lý, đọc thanh ghi/chuỗi.
3. Viết script **Frida** hook `strcmp` (hoặc hàm của app) và chạy `frida`.
4. Chạy `fs_usage` và `log stream --predicate` để quan sát hành vi.
5. Tổng hợp thành báo cáo dynamic.

## Liên kết code mẫu

Xem [code/week05/README.md](../code/week05/README.md) — `frida_hook_template.js`, `dynamic_log_parser.py`.

## Thảo luận

- LLDB và Frida bổ sung cho nhau thế nào trong dynamic analysis?
- Vì sao cần chạy app trong VM khi dùng Frida trên app lạ?

## Bài tập

- **Cơ bản:** Liệt kê 5 lệnh LLDB cơ bản và chức năng.
- **Nâng cao:** Viết script Frida hook một hàm của app bạn viết và log tham số.
- **Thử thách:** Dùng LLDB để thay đổi giá trị trả về của một hàm ngay khi đang chạy.

## Nộp bài và rubric

Nộp báo cáo dynamic + script Frida + output LLDB/fs_usage. Rubric chuẩn 100 điểm.

> **Cảnh báo pháp lý:** chỉ hook app của chính bạn hoặc app được cấp phép trong VM lab. Không dùng Frida/LLDB để né license hoặc lấy dữ liệu của app thương mại trái phép. Tuần 06 đi sâu vào hooking runtime.
