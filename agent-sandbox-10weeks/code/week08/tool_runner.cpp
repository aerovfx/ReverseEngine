// ============================================================================
// 08_tool_runner.cpp — Wrapper chạy tool an toàn (whitelist + denylist + giới hạn)
//
// Mục tiêu: minh hoạ cách hạn chế rủi ro khi agent chạy lệnh: WHITELIST trước
//           (chỉ cho phép lệnh đã biết), DENYLIST bổ sung (chặn mẫu nguy hiểm),
//           kèm timeout + giới hạn tài nguyên (CPU/RAM) và bắt output để audit.
// Đầu vào : --cmd "<lệnh>" (mặc định demo).
// Đầu ra : quyết định cho phép/chặn + lý do.
// An toàn : KHÔNG chạy lệnh thật — chỉ ĐÁNH GIÁ chuỗi lệnh bằng quy tắc.
//
// Biên dịch : g++ -std=c++17 -Wall -Wextra -O2 tool_runner.cpp -o tool_runner
// ============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>

// Kết quả đánh giá một lệnh.
struct Verdict {
    bool ok;
    std::string reason;
};

// Wrapper "sandbox policy" cho lệnh: whitelist + denylist + giới hạn.
class ToolPolicy {
public:
    ToolPolicy() {
        // Whitelist: chỉ các lệnh cơ bản được phép.
        allowed_ = {"ls", "cat", "echo", "pwd", "python"};
        // Denylist regex: chặn các mẫu nguy hiểm dù lệnh gốc có trong whitelist.
        blocked_ = {
            std::regex(R"(rm\s+-rf)"),      // xoá đệ quy
            std::regex(R"(mkfs)"),           // format ổ đĩa
            std::regex(R"(dd\s+if=)"),       // ghi đĩa thô
            std::regex(R"(>\s*/dev/)"),      // ghi vào thiết bị
            std::regex(R"(chmod\s+777)"),    // mở quyền rộng
        };
    }

    // Đánh giá lệnh: whitelist -> denylist -> giới hạn tài nguyên.
    Verdict evaluate(const std::string& cmd) const {
        std::string base = cmd.substr(0, cmd.find(' '));   // tên lệnh
        if (!allowed_.count(base)) {
            return {false, "lệnh '" + base + "' không nằm trong whitelist"};
        }
        for (const auto& re : blocked_) {
            if (std::regex_search(cmd, re)) {
                return {false, "khớp mẫu nguy hiểm (denylist)"};
            }
        }
        // Giới hạn tài nguyên mô phỏng (trong thực tế là cgroup/VM limit).
        return {true, "hợp lệ — chạy trong sandbox với timeout=10s, mem=512MB"};
    }

private:
    std::unordered_set<std::string> allowed_;
    std::vector<std::regex> blocked_;
};

int main(int argc, char** argv) {
    std::string cmd = (argc > 1) ? argv[1] : "ls -la";
    ToolPolicy policy;

    std::cout << "TOOL RUNNER AN TOÀN — whitelist + denylist + giới hạn tài nguyên\n";
    std::cout << std::string(70, '=') << "\n\n";

    const std::vector<std::string> demo = {
        cmd,
        "ls -la",
        "cat notes.txt",
        "rm -rf /",
        "python script.py",
        "dd if=/dev/zero of=/dev/sda",
    };

    for (const std::string& c : demo) {
        Verdict v = policy.evaluate(c);
        std::cout << (v.ok ? "  [CHO PHÉP] " : "  [CHẶN]     ") << c << "\n";
        std::cout << "             → " << v.reason << "\n";
    }

    std::cout << "\n" << std::string(70, '-') << "\n";
    std::cout << "KẾT LUẬN: whitelist (mặc định chặn) + denylist (chặn mẫu nguy hiểm)\n";
    std::cout << "+ timeout + giới hạn CPU/RAM + output capture = chạy tool an toàn.\n";
    return 0;
}
