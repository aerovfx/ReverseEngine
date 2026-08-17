// ============================================================================
// 10_harden_check.cpp — Checklist đánh giá hardening + sinh syscall allowlist
//
// Mục tiêu: cho khung đánh giá mức độ an toàn của sandbox theo từng TẦNG
//           (cô lập, seccomp, storage, mạng, egress, secret, vận hành) và
//           sinh danh sách syscall allowlist (mô phỏng seccomp-BPF).
// Đầu vào : --pass "1,2,3" (các tầng đã đạt), tuỳ chọn.
// Đầu ra : bảng trạng thái hardening + điểm + syscall allowlist gợi ý.
// An toàn : THUẦN TÍNH TOÁN — chỉ đánh giá theo checklist, không đụng hệ thống.
//
// Biên dịch : g++ -std=c++17 -Wall -Wextra -O2 harden_check.cpp -o harden_check
// ============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sstream>

// Một tầng hardening: tên + mô tả.
struct Layer {
    std::string name;
    std::string desc;
};

// Đánh giá hardening: đánh dấu các tầng đã đạt, cho điểm.
class HardeningAudit {
public:
    HardeningAudit(std::set<int> passed) : passed_(std::move(passed)) {}

    bool passed(int idx) const { return passed_.count(idx) > 0; }
    int  score() const { return static_cast<int>(passed_.size()); }

private:
    std::set<int> passed_;
};

// Parse chuỗi "1,2,3" -> set<int>.
std::set<int> parse_passed(const std::string& s) {
    std::set<int> out;
    std::stringstream ss(s);
    std::string tok;
    while (std::getline(ss, tok, ',')) {
        if (!tok.empty()) out.insert(std::stoi(tok));
    }
    return out;
}

int main(int argc, char** argv) {
    // Chấp nhận dạng: ./harden_check 1,2,3  hoặc  ./harden_check --passed 1,2,3
    std::string pass_arg;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--passed" || a == "--pass") { continue; }  // bỏ qua cờ
        pass_arg = a;                                        // lấy giá trị danh sách
    }
    HardeningAudit audit(parse_passed(pass_arg));

    const std::vector<Layer> layers = {
        {"Cô lập",    "MicroVM (KVM), mỗi sandbox kernel riêng"},
        {"Seccomp",   "VMM bị giới hạn syscall tối thiểu"},
        {"Storage",   "CoW snapshot/clone O(1), rollback nhanh"},
        {"Mạng",      "Network policy default-deny"},
        {"Egress",    "Domain allowlist + TLS inspection"},
        {"Secret",    "Credential injection, secret không vào sandbox"},
        {"Vận hành",  "Auto-pause, audit log, giới hạn tài nguyên"},
    };

    std::cout << "HARDENING CHECKLIST\n" << std::string(70, '=') << "\n\n";
    for (size_t i = 0; i < layers.size(); ++i) {
        bool ok = audit.passed(static_cast<int>(i + 1));
        std::cout << (ok ? "  [ĐẠT] " : "  [   ] ") << (i + 1) << ". "
                  << layers[i].name << " — " << layers[i].desc << "\n";
    }

    std::cout << "\nĐiểm: " << audit.score() << "/" << layers.size() << "\n";

    // Gợi ý syscall allowlist (mô phỏng seccomp-BPF cho microVM).
    const std::vector<std::string> allowlist = {
        "read", "write", "close", "exit", "exit_group", "mmap", "munmap",
        "mprotect", "futex", "nanosleep", "clock_gettime",
    };
    std::cout << "\nSyscall allowlist gợi ý (seccomp cho microVM):\n  ";
    for (const std::string& sc : allowlist) std::cout << sc << " ";
    std::cout << "\n";

    std::cout << "\n" << std::string(70, '-') << "\n";
    std::cout << "MỤC TIÊU: nếu agent bị prompt injection, kẻ tấn công gần như\n";
    std::cout << "không làm được gì (default-deny ở mọi tầng).\n";
    return 0;
}
