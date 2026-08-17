// ============================================================================
// 02_isolation_levels.cpp — So sánh container vs VM vs MicroVM + syscall allowlist
//
// Mục tiêu: minh hoạ khái niệm CÔ LẬP theo hai trục:
//   (1) ba mức ảo hoá (container / VM / microVM) và trade-off tốc độ-cô lập;
//   (2) "syscall allowlist" — cơ chế seccomp thu hẹp bề mặt tấn công.
// Đầu vào : không (demo có sẵn).
// Đầu ra : bảng so sánh mức cô lập + bảng cho phép/chặn syscall.
// An toàn : THUẦN TÍNH TOÁN — chỉ mô phỏng allowlist bằng std::set,
//           KHÔNG gọi syscall thật, KHÔNG dùng seccomp/ptrace.
//
// Biên dịch : g++ -std=c++17 -Wall -Wextra -O2 isolation_levels.cpp -o isolation_levels
// ============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

// Một mức cô lập: tên + chia sẻ kernel? + overhead + bề mặt escape.
struct Level {
    std::string name;
    bool        shared_kernel;
    std::string overhead;
    std::string escape_surface;
};

// "Sandbox policy" mô phỏng seccomp: danh sách syscall ĐƯỢC PHÉP.
// Syscall ngoài danh sách -> bị chặn (default-deny).
class SyscallPolicy {
public:
    explicit SyscallPolicy(std::set<std::string> allowed)
        : allowed_(std::move(allowed)) {}

    // Trả về true nếu syscall được phép, false nếu bị chặn.
    bool allow(const std::string& syscall) const {
        return allowed_.count(syscall) > 0;
    }

private:
    std::set<std::string> allowed_;
};

int main() {
    // --- Phần 1: so sánh các mức cô lập ---
    const std::vector<Level> levels = {
        {"Container",            true,  "Thấp (~ms)",   "Lớn — kernel chung, syscall"},
        {"VM (QEMU/KVM)",        false, "Cao (giây)",   "Nhỏ — hypervisor"},
        {"MicroVM (RustVMM)",    false, "Rất thấp (<100ms)", "Nhỏ — VMM tối giản"},
    };

    std::cout << "SO SÁNH CÁC MỨC CÔ LẬP\n" << std::string(74, '=') << "\n\n";
    for (const Level& lv : levels) {
        std::cout << lv.name << "\n"
                  << "  Chia sẻ kernel : " << (lv.shared_kernel ? "CÓ (rủi ro kernel vuln)" : "KHÔNG (kernel riêng)") << "\n"
                  << "  Overhead       : " << lv.overhead << "\n"
                  << "  Escape surface : " << lv.escape_surface << "\n\n";
    }

    // --- Phần 2: syscall allowlist (mô phỏng seccomp) ---
    // Policy "microVM nghiêm ngặt": chỉ cho phép vài syscall vô hại.
    // Trong thực tế đây là seccomp-BPF; ở đây ta mô phỏng bằng std::set.
    const SyscallPolicy microvm_policy({
        "read", "write", "close", "exit", "mmap", "munmap"
    });

    const std::vector<std::string> attempts = {
        "read",     // vô hại -> cho phép
        "write",    // vô hại -> cho phép
        "ptrace",   // nguy hiểm -> chặn
        "mount",    // nguy hiểm -> chặn
        "kexec_load", // nguy hiểm -> chặn
    };

    std::cout << "SYSCALL ALLOWLIST (mô phỏng seccomp, default-deny)\n";
    std::cout << std::string(74, '-') << "\n";
    for (const std::string& sc : attempts) {
        bool ok = microvm_policy.allow(sc);
        std::cout << (ok ? "  [CHO PHÉP] " : "  [CHẶN]     ") << sc << "\n";
    }

    std::cout << "\nKẾT LUẬN: microVM = kernel riêng + VMM tối giản + syscall\n";
    std::cout << "allowlist — chính là cách CubeSandbox đạt 'hardware-level isolation'.\n";
    return 0;
}
