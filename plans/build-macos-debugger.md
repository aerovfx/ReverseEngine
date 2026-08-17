# Xây dựng debugger cho macOS (ARM64) tương tự x64dbg — tên làm việc `a64dbg`

## Bối cảnh

Người dùng muốn xây một trình gỡ lỗi (debugger) cho macOS có trải nghiệm tương tự x64dbg (trình debugger mã nguồn mở cho Windows, viết bằng C++ + Qt). Các quyết định đã chốt với người dùng:

| Quyết định | Lựa chọn |
|---|---|
| Ngôn ngữ / GUI | **C++ + Qt** (giống x64dbg, hiệu năng cao, dễ port) |
| Debug backend | **Hai tầng**: tự viết core bằng **Mach API** + tái dùng **LLDB SB API** cho symbolication/step phức tạp |
| Phạm vi | **Đầy đủ như x64dbg** (plugin SDK, scripting, watchpoint, trace, symbol/source view...) |
| Kiến trúc đích | **ARM64 (Apple Silicon) trước**; x86_64/Rosetta để dành giai đoạn sau |

x64dbg gốc là family gồm x64dbg/x32dbg (disassembler Capstone + assembler + TitanEngine debug engine + Qt GUI + plugin SDK + scripting). `a64dbg` ánh xạ 1-1 mô hình đó sang macOS ARM64: **Capstone** (AArch64 disasm), **Keystone** (assemble), **Mach exception port** (thay TitanEngine) cho core, và **LLDB SB API** làm tầng bổ trợ.

> **Ranh giới pháp lý / an toàn:** debugger là công cụ hợp pháp phục vụ học reverse engineering, phân tích malware trong lab cô lập và gỡ lỗi phần mềm của chính mình. Kế hoạch này chỉ xây dựng công cụ, không kèm nội dung hướng dẫn crack/phá DRM trái phép. Việc attach vào tiến trình người khác chịu ràng buộc của entitlement `com.apple.security.cs.debugger`, SIP và sự đồng ý của chủ sở hữu máy.

## Mục tiêu & phạm vi

**Kết quả cuối:** một ứng dụng `.app` chạy native trên Apple Silicon, code-signed với entitlement debugger, gỡ lỗi được một chương trình ARM64 do chính người dùng build, với đầy đủ view và khả năng mở rộng bằng plugin/script như x64dbg.

Ma trận tính năng (ánh xạ x64dbg → nguồn thực hiện):

| Nhóm | Tính năng | Nguồn thực hiện |
|---|---|---|
| Phiên | open/attach/detach/restart/kill | Mach core (task_for_pid, launch-suspend) |
| Breakpoint | software (BRK patch), hardware (bvr/bcr), watchpoint (wvr/wcr) | Mach core (ARM_DEBUG_STATE64) |
| Step | step into/over/out, run-to-cursor, trace | step into: hardware SS; over/out: LLDB-assisted |
| View | CPU/disasm, dump (hex+ASCII), stack, registers, modules, symbols, threads, references, watch, memory map | Qt widgets + core/LLDB data |
| Tìm kiếm | byte pattern, string, referenced function, intermodular call | core (mem scan) + LLDB (symbol) |
| Sửa | assemble (Keystone), nop fill, paste bytes, save patch | core (mach_vm_write) |
| Ghi chú | comment, label, bookmark (persist ra file db) | GUI + lưu JSON/SQLite |
| Mở rộng | plugin SDK (C ABI, dlopen), scripting (Lua), command input | riêng |
| Symbol | demangle, source line, dSYM/DWARF | LLDB layer (dyld shared cache, DWARF) |

**Ngoài phạm vi giai đoạn này (defer):** x86_64/Rosetta 2, debug kernel, graph view (call graph), remote debug (debugserver protocol), multi-process attach.

## Kiến trúc tổng thể

```
┌─────────────────────────────────────────────────────────────┐
│  GUI (Qt 6 Widgets, main thread)                             │
│  DisasmView · DumpView · StackView · RegisterView · ...      │
│  BreakpointManager · SymbolProvider · PluginHost · Script    │
└───────────────▲─────────────────────────────▲────────────────┘
                │ Qt queued events (thread-safe queue)          │
┌───────────────┴───────────────┐ ┌─────────────┴───────────────┐
│  IDebugBackend (interface)     │ │  ISymbolProvider (iface)    │
└───────────────┬───────────────┘ └─────────────┬───────────────┘
        ┌───────┴────────┐                ┌──────┴───────┐
        │ MachBackend    │                │ LLDBBackend  │
        │ (tự viết, core)│                │ (SB API)     │
        └───────┬────────┘                └──────────────┘
   ┌────────────┼─────────────────────────────┐
   │ task_for_pid · exception port · mach_vm  │
   │ thread_get/set_state · arm_debug_state64 │
   └────────────┴─────────────────────────────┘
   Capstone (disasm) · Keystone (assemble) · Unicorn (emulate, tùy chọn)
```

**Nguyên tắc kiến trúc:**
1. **Tách backend khỏi GUI** qua interface thuần C++ (`IDebugBackend`, `ISymbolProvider`). GUI không biết đang chạy Mach hay LLDB.
2. **Hai tầng backend** — Mach core sở hữu các primitive nó làm tốt (attach, memory, register, breakpoint phần cứng/mềm, exception); LLDB layer đảm nhận thứ khó làm đúng từ đầu (symbolication dyld shared cache/DWARF, step over/out, biểu thức `expr`). Hai tầng cùng mở trên một `task_t`/pid; core là nguồn sự thật về trạng thái dừng.
3. **Luồng event bất đồng bộ:** một thread riêng chạy vòng `mach_msg` nhận exception, chuẩn hóa thành `DebugEvent` (stop reason, thread, address, type), đẩy vào hàng đợi thread-safe; GUI nhận qua Qt queued connection. Tránh deadlock: khi nhận exception phải **dừng-toàn-cụm** (suspend tất cả thread khác) trước khi đọc state, rồi mới cho GUI xử lý.
4. **Chia nhỏ theo module một-chủ-một-thư-mục** (tham chiếu skill `realtime-subsystem-architecture` khi tới bước viết code): mỗi subsystem là một thư viện CMake riêng, cấm import chéo vòng.

## Tech stack & dependency

| Thành phần | Lựa chọn | Lý do |
|---|---|---|
| Build | CMake + Ninja, presets | chuẩn, dễ cross-check (skill `cmake-c-cpp-crossbuild` áp dụng khi dựng) |
| GUI | Qt 6 (Widgets) | QDockWidget + QAbstractScrollArea phù hợp view kiểu x64dbg |
| Disassembler | Capstone (CS_ARCH_ARM64) | cùng engine x64dbg dùng, hỗ trợ AArch64 tốt |
| Assembler | Keystone (CS_ARCH_ARM64) | cùng họ Capstone, API đồng bộ |
| Debug core | Mach API hệ thống (không dependency ngoài) | task_for_pid, exception port, thread state, mach_vm |
| Symbol/step | LLDB (headers + liblldb, SB API) | demangle, dSYM/DWARF, dyld shared cache, step over/out, expr |
| Emulator (tùy chọn) | Unicorn | fallback step-over khi không muốn phụ thuộc LLDB |
| Scripting | Lua (lua 5.4 + sol2) — dự phòng QJSEngine | nhỏ, nhanh, quen thuộc với cộng đồng RE |
| Plugin | C ABI + `dlopen` | tương đương plugin SDK x64dbg |
| Dependency mgmt | FetchContent + Homebrew | giữ tái lập được trên máy dev |

## Cấu trúc thư mục (dự kiến khi triển khai)

```
a64dbg/
  CMakeLists.txt · CMakePresets.json · vcpkg.json (hoặc FetchContent)
  src/
    app/            # main(), MainWindow, theme
    core/           # MachBackend: attach, exception, memory, thread, regs
    lldb/           # LLDBBackend, symbol provider, step, expr
    disasm/         # Capstone wrapper + instruction cache + decomposer
    asm/            # Keystone wrapper
    model/          # BreakpointManager, ModuleManager, Session, settings
    ui/             # DisasmView, DumpView, StackView, RegisterView, ...
    plugin/         # PluginHost, dbg_sdk.h, lua scripting
    common/         # event queue, logger, mach-o parser, json/db persist
  include/dbg_sdk.h # header public cho plugin
  tests/            # unit + integration (CTest)
  plugins/          # plugin mẫu (example_plugin)
  scripts/          # script mẫu
  docs/             # ARCHITECTURE.md, features.md, PLUGINS.md
```

## Lộ trình triển khai (phase + dependency)

> Mỗi phase kết thúc bằng một build chạy được + checklist tự kiểm (B2) trước khi sang phase sau. Không viết code khi spec phase đó chưa chốt.

**Phase 0 — Skeleton & build (tuần 1–2)**
- CMake + Qt skeleton, layout dock 4 khung (disasm/registers/dump/stack) rỗng, dark theme.
- Kéo dependency Capstone/Keystone/LLDB về, build sạch trên arm64.
- Code-sign với entitlement `com.apple.security.cs.debugger`; CI (GitHub Actions macOS arm64) build + smoke test.
- *Dependency:* không có. *Chốt:* quy ước coding style, cấu trúc module.

**Phase 1 — Mach core: attach & điều khiển (tuần 3–4)**
- Attach/detach bằng `task_for_pid`; launch child bằng `posix_spawn(POSIX_SPAWN_START_SUSPENDED)`.
- Exception port (`task_set_exception_ports` + vòng `mach_msg`) cho EXC_BREAKPOINT/EXC_BAD_ACCESS/EXC_SINGLE_STEP/EXC_ARITHMETIC.
- `thread_suspend/resume`, đọc/ghi `ARM_THREAD_STATE64` (x0–x29, fp, lr, sp, pc, cpsr), `mach_vm_read_overwrite/write`.
- Đẩy `DebugEvent` lên GUI; hiển thị thô (log) để chứng minh bắt được event.
- *Chốt:* contract `IDebugBackend` (danh sách method, kiểu event, ngữ nghĩa stop-the-world).

**Phase 2 — Disassembly & CPU view cơ bản (tuần 5–6)**
- Capstone wrapper + instruction cache (invalidate khi byte đổi).
- Parse Mach-O (`LC_SEGMENT_64`, `LC_SYMTAB`, `LC_FUNCTION_STARTS`, `LC_DYSYMTAB`) → module list + memory map.
- DisasmView (cuộn, follow RIP→PC, highlight breakpoint/PC), RegisterView, DumpView (hex+ASCII), StackView sơ khai.
- *Dependency:* Phase 1. *Chốt:* behavior "follow in dump / follow in disasm".

**Phase 3 — Breakpoint & stepping (tuần 7–8)**
- Software BP: patch `BRK #imm` (0xD4200000), lưu byte gốc, toggle/xóa, danh sách.
- Hardware BP/watchpoint qua `ARM_DEBUG_STATE64` (`bvr[16]/bcr[16]`, `wvr[16]/wcr[16]`).
- Step into bằng hardware single-step (MDSCR_EL1.SS); step over/out giao LLDB.
- Run-to-cursor; tiếp tục (continue) với khôi phục BRK đúng cách (step rồi repatch — kỹ thuật "breakpoint dance").
- *Dependency:* Phase 1, 2. *Chốt:* hành vi first-chance/last-chance exception.

**Phase 4 — LLDB layer & symbols (tuần 9–10)**
- Triển khai `LLDBBackend` + `ISymbolProvider`: demangle, tên hàm/source line, dSYM/DWARF, dyld shared cache.
- View modules/symbols; expression evaluation (`expr`) qua SB API.
- *Dependency:* Phase 1 (để core vẫn chạy độc lập). *Chốt:* cách core và LLDB chia sẻ trạng thái dừng.

**Phase 5 — View nâng cao & tìm kiếm (tuần 11–12)**
- Stack view đầy đủ (walk theo fp/lr + unwind), watch view, threads view, references view, memory map view.
- Tìm byte pattern/string; referenced function; comment/label/bookmark persist ra file.
- Assemble (Keystone), nop fill, paste bytes, save patch.
- *Dependency:* Phase 2–4.

**Phase 6 — Plugin SDK & scripting (tuần 13–14)**
- `dbg_sdk.h` C ABI; PluginHost load qua `dlopen`; callback (INITDEBUG, STOPDEBUG, EXCEPTION, BREAKPOINT, STEPPED, EXITPROCESS, MENUENTRY...); menu tích hợp.
- Nhúng Lua + bind API (đọc/ghi memory, reg, set bp, disasm); script console; command input bar.
- *Dependency:* Phase 3–5.

**Phase 7 — Hardening, packaging, polish (tuần 15–16)**
- Chống deadlock/self-debug, thread-safety event queue, resume đúng thread khi nhiều thread dừng.
- Đóng gói `.app` + dmg, notarization (adhoc cho dev), cập nhật ARCHITECTURE.md, plugin/script mẫu, docs.
- *Dependency:* toàn bộ.

**Phase C — Đánh giá độc lập & phản biện chéo (sau Phase 7)** — theo skill `ai-dev-workflow`:
- C1: Reviewer #1 (model/phiên khác) đọc toàn project, chấm kiến trúc/chất lượng/bảo mật/hiệu năng/bảo trì thang 100, lưu báo cáo riêng (`reports/`).
- C2: Reviewer #2 tự đánh giá **trước** khi đọc báo cáo #1, rồi đối chiếu từng nhận xét, tìm thêm vấn đề, xuất báo cáo riêng.
- C3: Tổng hợp thành lộ trình fix ưu tiên (điểm cả hai đồng ý xử lý trước; điểm bất đồng tự kiểm chứng bằng chạy thử).

## Ràng buộc không được phá

- **Entitlement & ký:** app phải ký với `com.apple.security.cs.debugger` (hoặc chạy root) mới gọi `task_for_pid` lên tiến trình khác; với tiến trình do mình launch thì dùng `posix_spawn` suspend để không cần quyền đặc biệt cho bản demo.
- **SIP:** không attach được tiến trình Apple ký hệ thống khi SIP bật — đây là giới hạn nền tảng, phải ghi rõ trong docs, không tìm cách né.
- **Stop-the-world:** trước khi đọc state khi dừng phải suspend đủ các thread; tránh chạy GUI call trực tiếp vào Mach API từ thread `mach_msg` (deadlock).
- **ARM64 đúng chuẩn:** disasm/assemble dùng CS_ARCH_ARM64; không giả định encoding x86; `pc` trỏ đúng ngữ nghĩa (PC-relative trong AArch64 khác x86).
- **Không tự đổi kiến trúc giữa chừng:** mọi đổi kiến trúc phải dừng xin xác nhận người dùng (nguyên tắc skill).
- **Pháp lý:** chỉ xây công cụ debug hợp pháp; không kèm hướng dẫn crack/DRM lậu; mọi tài liệu có cảnh báo phạm vi sử dụng.

## Rủi ro & cách giảm thiểu

| Rủi ro | Mức | Giảm thiểu |
|---|---|---|
| Single-step ARM64 (MDSCR_EL1.SS) có quirk với một số instruction/qua dyld | Cao | step into dùng hardware SS; over/out giao LLDB; fallback Unicorn emulate |
| Symbolication dyld shared cache từ Mach-O thô rất khó | Cao | tầng LLDB đảm nhận (đã chốt hai tầng) |
| Deadlock giữa vòng mach_msg và suspend/resume | Cao | một thread chủ exception + stop-the-world chuẩn + unit test stress |
| `task_for_pid` bị chặn (entitlement/SIP) | Trung | ký entitlement từ đầu; demo dùng launch-suspend; docs nêu giới hạn |
| Kết hợp hai backend lệch trạng thái dừng | Trung | Mach core là nguồn sự thật; LLDB chỉ đọc/step trên pid đã đồng bộ |
| Phạm vi "đầy đủ như x64dbg" quá lớn | Trung | chia 8 phase, mỗi phase có build chạy được; plugin/script tới Phase 6 để lõi sớm ổn định |
| x86_64/Rosetta chưa làm | Thấp | defer rõ ràng; Capstone/Keystone đã hỗ trợ x86 nên chỉ thêm backend sau |

## Cách kiểm chứng (mỗi phase + tổng thể)

- **Build:** `cmake --preset` + `ninja` xanh trên macOS arm64; CI GitHub Actions chạy.
- **Đơn vị (CTest):** parser Mach-O, instruction cache, patch BRK khôi phục đúng byte, đọc/ghi register trên binary mẫu.
- **Tích hợp:** binary ARM64 mẫu (tự build, có hàm target) → attach → đặt BP phần mềm → chạy tới → đọc `x0`/`pc` đúng giá trị kỳ vọng → step vào → dump vùng nhớ khớp.
- **Ma trận tính năng:** đối chiếu checklist x64dbg ở trên, đánh dấu done/LLDB-assisted/defer.
- **An toàn:** chạy debugger tự attach chính nó không treo; stress 1000 breakpoint liên tiếp không rò memory (Leaks/ASan).
- **Review:** đủ báo cáo C1 + C2, lộ trình fix C3, chấm điểm đạt ngưỡng người dùng chốt (gợi ý 80/100) trước khi phát hành.

## Phương án đã cân nhắc và loại bỏ

- **Dùng LLDB làm backend duy nhất:** loại vì người dùng muốn tự viết core Mach (giá trị học reverse engineering); giữ lại làm tầng symbol/step.
- **Thuần Mach API 100%:** loại vì symbolication dyld/DWARF và step over/out từ đầu quá nặng, dễ sai âm thầm; người dùng chọn phương án hai tầng.
- **Swift/SwiftUI:** loại vì người dùng chọn C++/Qt (port + thư viện RE).
- **Rust/egui:** loại vì hệ sinh thái debug macOS non hơn, người dùng chọn C++/Qt.
- **Electron/Web:** loại vì hiệu năng view hex/disasm lớn kém và xa hệ sinh thái RE native.
- **Frida làm lõi:** loại vì Frida là instrumentation framework (hook/trace) chứ không phải interactive debugger kiểu x64dbg; có thể dùng làm plugin sau này.
- **x86_64 trước:** loại vì máy đích Apple Silicon, ARM64 là mặc định; x86_64/Rosetta defer.
