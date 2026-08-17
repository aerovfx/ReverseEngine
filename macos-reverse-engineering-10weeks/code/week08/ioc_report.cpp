// 08_ioc_report.cpp — Sinh báo cáo IOC macOS (persistence + mạng + file)
// Mục tiêu: tổng hợp dấu hiệu xâm nhập (LaunchAgent, file, domain) thành báo cáo.
// An toàn: thuần tính toán, chỉ in văn bản.  g++ -std=c++17 -O2 ioc_report.cpp -o demo
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::pair<std::string,std::string>> iocs = {
        {"LaunchAgent", "~/Library/LaunchAgents/com.attacker.agent.plist"},
        {"File",        "/tmp/payload.dylib"},
        {"Domain",      "evil.example"},
        {"Hash",        "7f83b1657ff1fc53"},
    };
    std::cout << "BÁO CÁO IOC — macOS\n" << std::string(56,'=') << "\n\n";
    for (auto& [loai, giatri] : iocs) std::cout << "  [" << loai << "] " << giatri << "\n";
    std::cout << "\nKhắc phục: xoá LaunchAgent, chặn domain, xoá snapshot VM.\n";
    return 0;
}
