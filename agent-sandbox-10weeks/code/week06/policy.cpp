// ============================================================================
// 06_policy.cpp — Network policy (CIDR longest-prefix-match, default-deny)
//
// Mục tiêu: minh hoạ cách CubeVS (eBPF) khớp network policy theo CIDR bằng
//           LONGEST-PREFIX-MATCH (LPM): luật có prefix dài hơn được ưu tiên.
// Đầu vào : không (policy + tập địa chỉ demo có sẵn).
// Đầu ra : kết quả cho phép/chặn cho từng địa chỉ đích.
// An toàn : THUẦN TÍNH TOÁN — phân tích IP dạng số, không gửi gói tin thật.
//
// Biên dịch : g++ -std=c++17 -Wall -Wextra -O2 policy.cpp -o policy
// ============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>

// Một luật policy: prefix (CIDR) + hành động allow/deny.
struct Rule {
    uint32_t net;      // địa chỉ mạng (dạng uint32)
    int      bits;     // độ dài prefix
    bool     allow;    // true = allow, false = deny
};

// Chuyển "a.b.c.d" -> uint32 (host byte order để dễ so sánh).
uint32_t ip_to_u32(const std::string& ip) {
    uint32_t r = 0;
    int octet = 0, shift = 24;
    for (char c : ip + '.') {
        if (c == '.') { r |= (uint32_t(octet) << shift); octet = 0; shift -= 8; }
        else { octet = octet * 10 + (c - '0'); }
    }
    return r;
}

// Parse "net/bits" -> Rule.
Rule parse_rule(const std::string& cidr, bool allow) {
    auto pos = cidr.find('/');
    uint32_t net = ip_to_u32(cidr.substr(0, pos));
    int bits = std::stoi(cidr.substr(pos + 1));
    return {net, bits, allow};
}

// Khớp một IP với danh sách luật theo longest-prefix-match (default-deny).
bool match(const std::vector<Rule>& rules, const std::string& ip) {
    uint32_t addr = ip_to_u32(ip);
    int best_bits = -1;
    bool best_allow = false;
    for (const Rule& r : rules) {
        int shift = 32 - r.bits;
        uint32_t mask = shift >= 32 ? 0 : (0xFFFFFFFFu << shift);
        if ((addr & mask) == (r.net & mask)) {
            if (r.bits > best_bits) {   // prefix dài hơn -> ưu tiên hơn
                best_bits = r.bits;
                best_allow = r.allow;
            }
        }
    }
    return best_bits >= 0 ? best_allow : false;   // không khớp -> default-deny
}

int main() {
    // Policy mặc định: cho phép mạng nội bộ, chặn mọi thứ khác.
    std::vector<Rule> rules = {
        parse_rule("10.0.0.0/8", true),   // nội bộ riêng
        parse_rule("0.0.0.0/0", false),   // default-deny phần còn lại
    };

    const std::vector<std::string> tests = {
        "10.1.2.3",     // nội bộ -> allow
        "192.168.1.1",  // không khớp -> deny
        "8.8.8.8",      // internet -> deny
    };

    std::cout << "NETWORK POLICY — longest-prefix-match, default-deny\n";
    std::cout << std::string(70, '=') << "\n\n";
    std::cout << "Policy:\n";
    for (const Rule& r : rules) {
        std::cout << "  " << (r.allow ? "ALLOW" : "DENY ") << " prefix=" << r.bits << "\n";
    }
    std::cout << "\n";
    for (const std::string& ip : tests) {
        bool ok = match(rules, ip);
        std::cout << "  đích " << ip << "  -> " << (ok ? "ALLOW" : "DENY") << "\n";
    }

    std::cout << "\n" << std::string(70, '-') << "\n";
    std::cout << "KẾT LUẬN: LPM cho phép khớp prefix dài nhất ở tốc độ cao (eBPF),\n";
    std::cout << "không cần duyệt từng luật iptables cho mỗi sandbox.\n";
    return 0;
}
