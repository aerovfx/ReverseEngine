// 05_log_parser.cpp — Phân tích unified log (subsystem/category) — mô phỏng
// Mục tiêu: hiểu cách đọc log stream để quan sát hành vi app lúc chạy.
// An toàn: thuần tính toán trên chuỗi demo.  g++ -std=c++17 -O2 log_parser.cpp -o demo
#include <iostream>
#include <string>
#include <vector>
#include <map>

struct Entry { std::string subsystem, category, message; };

int main() {
    std::vector<Entry> log = {
        {"com.app.malware", "network", "connect c2.example:443"},
        {"com.app.malware", "persistence", "install LaunchAgent"},
        {"com.apple.system", "tcc", "access Contacts"},
        {"com.app.malware", "network", "upload 4KB"},
    };
    std::map<std::string,int> cnt;
    for (auto& e : log) cnt[e.category]++;

    std::cout << "UNIFIED LOG — phân tích động\n" << std::string(56,'=') << "\n\n";
    for (auto& [cat, n] : cnt) std::cout << "  [" << cat << "] " << n << " sự kiện\n";

    std::cout << "\nSự kiện khả nghi:\n";
    for (auto& e : log)
        if (e.subsystem == "com.app.malware") std::cout << "  - " << e.category << ": " << e.message << "\n";
    return 0;
}
