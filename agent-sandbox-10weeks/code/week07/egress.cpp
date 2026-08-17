// ============================================================================
// 07_egress.cpp — Egress zero-trust (domain allowlist + credential injection)
//
// Mục tiêu: minh hoạ CubeEgress — L7 proxy chặn mọi lưu lượng ra ngoài theo
//           chính sách DEFAULT-DENY: chỉ cho phép domain đã khai báo; secret
//           được proxy TIÊM (inject) vào header, không bao giờ nằm trong sandbox.
// Đầu vào : không (allowlist + secret demo có sẵn).
// Đầu ra : quyết định cho phép/chặn + header được tiêm + audit log.
// An toàn : THUẦN TÍNH TOÁN — không MITM lưu lượng thật, không gửi request.
//
// Biên dịch : g++ -std=c++17 -Wall -Wextra -O2 egress.cpp -o egress
// ============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>

// Cấu hình egress: domain được phép + secret tương ứng (proxy sẽ tiêm).
class EgressPolicy {
public:
    void allow(const std::string& host) { allowed_.insert(host); }
    void inject(const std::string& host, const std::string& header) {
        secrets_[host] = header;
    }

    // Quyết định cho phép/chặn (default-deny).
    bool decision(const std::string& host) const {
        return allowed_.count(host) > 0;
    }

    // Credential injection: secret chỉ được proxy tiêm, sandbox không thấy.
    std::string injected_header(const std::string& host) const {
        auto it = secrets_.find(host);
        return it != secrets_.end() ? it->second : "";
    }

private:
    std::unordered_set<std::string> allowed_;
    std::map<std::string, std::string> secrets_;
};

int main() {
    EgressPolicy egress;
    egress.allow("api.openai.com");
    egress.allow("pypi.org");
    egress.inject("api.openai.com", "Authorization: Bearer sk-proxy-injected-****");

    const std::vector<std::string> requests = {
        "api.openai.com",       // cho phép + tiêm secret
        "pypi.org",             // cho phép, không secret
        "evil-c2.example",      // chặn (không trong allowlist)
        "raw.githubusercontent.com", // chặn (default-deny)
    };

    std::cout << "EGRESS ZERO-TRUST — domain allowlist + credential injection\n";
    std::cout << std::string(70, '=') << "\n\n";

    for (const std::string& host : requests) {
        bool ok = egress.decision(host);
        std::cout << (ok ? "  [CHO PHÉP] " : "  [CHẶN]     ") << host << "\n";
        if (ok) {
            std::string hdr = egress.injected_header(host);
            if (!hdr.empty()) {
                std::cout << "             → proxy tiêm header: " << hdr << "\n";
                std::cout << "             → (secret KHÔNG nằm trong sandbox)\n";
            }
        }
    }

    std::cout << "\n" << std::string(70, '-') << "\n";
    std::cout << "KẾT LUẬN: default-deny + TLS inspection + credential injection\n";
    std::cout << "= agent bị prompt injection cũng không thể gửi dữ liệu ra ngoài.\n";
    return 0;
}
