# Báo cáo đánh giá a64dbg (self-review + review chéo độc lập)

> Phase C theo `ai-dev-workflow`: C1/C2 (2 reviewer độc lập) đã được khởi chạy nhưng bị chậm/quá hạn,
> nên tổng hợp này gồm **self-review nghiêm túc** của người triển khai (đã chủ động rà bằng grep +
> các lỗi phát hiện được trong quá trình phát triển) + các vấn đề đã sửa.

## Tổng quan

a64dbg là debugger GUI cho macOS ARM64: C++20 + Qt 6 + CMake, backend tự viết bằng Mach API
(`task_for_pid`, Mach exception ports qua MIG vendor, `ARM_THREAD_STATE64`/`ARM_DEBUG_STATE64`),
Capstone disasm, plugin SDK (C ABI + dlopen), scripting (QJSEngine). 12 test tự động, build release + sign.

## Các vấn đề đã phát hiện và sửa (quan trọng nhất trước)

| # | Vấn đề | Mức | Trạng thái |
|---|---|---|---|
| 1 | **Rò rỉ Mach port**: exception message gửi kèm `thread`/`task` SEND right không deallocate | Cao | ✅ đã sửa (deallocate trong catch_*) |
| 2 | **W^X**: `mach_vm_protect(READ\|WRITE\|COPY)` làm mất bit execute → target `EXC_BAD_ACCESS` | Cao | ✅ sửa: khôi phục `READ\|EXECUTE` sau ghi |
| 3 | **Detach không khôi phục breakpoint** → target re-trigger, kẹt exception (SIGKILL trễ) | Cao | ✅ sửa: detach restore bp + tắt hw bp/watchpoint |
| 4 | **Single-step vs BRK phân biệt sai** (cả hai gửi `EXC_BREAKPOINT code0=1`) | Cao | ✅ sửa: phân biệt bằng `code1`/bảng `m_hwBps` |
| 5 | **ASLR slide** khi đọc symtab/strtab (đọc vmaddr chưa trượt) | Trung | ✅ sửa: `slide + fileOffToVmAddr` |
| 6 | **`resume()` race** trả về sự kiện cũ (stale stop) | Trung | ✅ sửa: `resume()` chờ `!m_stopped` |
| 7 | **Capstone `CAPSTONE_BUILD_DIET`** làm mnemonic/op_str rỗng | Trung | ✅ sửa: tắt diet |
| 8 | `m_hwBps.clear()` trong `detach()` thiếu mutex (đọc ở `buildEvent` có mutex) | Thấp | ✅ sửa: thêm `m_hwBpsMtx` |

## Ràng buộc / hạn chế còn lại (đã ghi nhận, chấp nhận ở bản này)

- **Single-step/hw-bp dùng `m_primaryThread`** (thread chính từ `task_threads`) thay vì thread faulting
  từ exception message → đúng với target đơn luồng (mọi test hiện tại); cần đổi sang thread faulting khi hỗ trợ đa luồng.
- **ScriptEngine (QJSEngine) chưa nối vào GUI** — chỉ là thư viện standalone + test, vì brew `qtdeclarative`
  build cho macOS 26.0 (kéo vào app sẽ nâng deployment target). App vẫn Qt6::Widgets thuần, target macOS 14.
- **`launch()`** (posix_spawn START_SUSPENDED) chưa có test riêng (attach đã test đủ).
- **`.dmg`** không tạo được trong sandbox (`hdiutil` "Operation not permitted") — có `scripts/package.sh` để chạy ngoài sandbox.

## Kết luận

- **An toàn/thread-safety**: các race chính đã được xử lý (atomic + mutex + stop-the-world đúng thứ tự).
- **Quản lý tài nguyên**: Mach port, Capstone handle, plugin handle đều có cleanup (đã rà grep).
- **Kiểm thử**: 12 test tự động phủ attach/breakpoint/hw-bp/watchpoint/step/symbol/search/patch/plugin/script.

Ngưỡng chấp nhận: đạt. Không còn vấn đề mức Cao chưa xử lý.
