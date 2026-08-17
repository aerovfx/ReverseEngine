// ============================================================================
// 04_snapshot.cpp — Mô phỏng snapshot/restore bộ nhớ để khởi động microVM nhanh
//
// Mục tiêu: minh hoạ vì sao RESTORE nhanh hơn COLD BOOT:
//   - cold boot  = khởi tạo kernel + init (tốn thời gian);
//   - snapshot   = lưu trạng thái "nóng" (thanh ghi + bộ nhớ dirty);
//   - restore    = nạp lại snapshot, không boot lại từ đầu.
//   Kèm khái niệm dirty-page tracking: chỉ lưu trang đã thay đổi.
// Đầu vào : không.
// Đầu ra : so sánh thời gian (mô phỏng) + trạng thái trước/sau restore.
// An toàn : THUẦN TÍNH TOÁN — mô phỏng bộ nhớ bằng std::vector<uint8_t>.
//
// Biên dịch : g++ -std=c++17 -Wall -Wextra -O2 snapshot.cpp -o snapshot
// ============================================================================

#include <iostream>
#include <vector>
#include <cstdint>
#include <chrono>
#include <thread>

// Trạng thái một microVM: thanh ghi + một vùng bộ nhớ giả lập.
struct VmState {
    uint64_t rip = 0x1000;     // instruction pointer
    uint64_t rsp = 0x7fff0000; // stack pointer
    std::vector<uint8_t> mem;  // bộ nhớ (mỗi byte là một "trang" đơn giản hoá)
    bool dirty = false;        // đánh dấu có trang nào thay đổi sau snapshot không
};

// Snapshot: lưu TRẠNG THÁI (chỉ các trang dirty trong thực tế; ở đây lưu toàn bộ
// để đơn giản, nhưng ta mô phỏng khái niệm dirty qua cờ dirty_).
struct Snapshot {
    uint64_t rip, rsp;
    std::vector<uint8_t> mem;
    explicit Snapshot(const VmState& s) : rip(s.rip), rsp(s.rsp), mem(s.mem) {}
};

// Cold boot: khởi tạo lại kernel + init (mô phỏng tốn thời gian).
VmState cold_boot() {
    std::this_thread::sleep_for(std::chrono::milliseconds(60)); // giả lập ~giây thật
    VmState s;
    s.mem.assign(256, 0x00);   // RAM được zero-fill lúc boot
    return s;
}

// Restore: nạp snapshot (mô phỏng chỉ mất vài ms).
VmState restore(const Snapshot& snap) {
    std::this_thread::sleep_for(std::chrono::milliseconds(2)); // giả lập ~ms thật
    VmState s;
    s.rip = snap.rip;
    s.rsp = snap.rsp;
    s.mem = snap.mem;
    s.dirty = false;
    return s;
}

// Ghi vào bộ nhớ -> đánh dấu dirty (trong thực tế chỉ trang đó được snapshot lại).
void write_page(VmState& s, size_t idx, uint8_t value) {
    if (idx < s.mem.size()) {
        s.mem[idx] = value;
        s.dirty = true;
    }
}

int main() {
    using Clock = std::chrono::steady_clock;

    std::cout << "SNAPSHOT / RESTORE — vì sao microVM khởi động <100ms\n";
    std::cout << std::string(70, '=') << "\n\n";

    // 1. Cold boot lần đầu.
    auto t0 = Clock::now();
    VmState vm = cold_boot();
    auto cold_ms = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - t0).count();
    std::cout << "[cold boot] rip=" << std::hex << vm.rip
              << " mem[0]=" << (int)vm.mem[0] << std::dec
              << "  (mất " << cold_ms << "ms mô phỏng)\n";

    // 2. Chương trình chạy, ghi vài trang (đánh dấu dirty).
    write_page(vm, 0, 0xAB);
    write_page(vm, 10, 0xCD);

    // 3. Snapshot trạng thái "nóng".
    Snapshot snap(vm);
    std::cout << "[snapshot ] đã lưu trạng thái (rip=" << std::hex << vm.rip
              << ", dirty=" << (vm.dirty ? "true" : "false") << std::dec << ")\n";

    // 4. Restore từ snapshot (nhanh hơn nhiều).
    auto t1 = Clock::now();
    VmState vm2 = restore(snap);
    auto restore_ms = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - t1).count();
    std::cout << "[restore  ] rip=" << std::hex << vm2.rip
              << " mem[0]=0x" << (int)vm2.mem[0]
              << " mem[10]=0x" << (int)vm2.mem[10] << std::dec
              << "  (mất " << restore_ms << "ms mô phỏng)\n";

    std::cout << "\n" << std::string(70, '-') << "\n";
    std::cout << "KẾT LUẬN: restore = nạp snapshot (" << restore_ms << "ms) nhanh hơn\n";
    std::cout << "nhiều so với cold boot (" << cold_ms << "ms). CubeSandbox dùng template\n";
    std::cout << "đã snapshot sẵn + RustVMM restore để đạt <100ms.\n";
    return 0;
}
