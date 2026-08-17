// 01_lab_checklist.cpp — Checklist RoE + lab cô lập (Windows RE)
// Mục tiêu: sinh Rules of Engagement + checklist lab trước khi phân tích binary.
// An toàn: thuần tính toán, chỉ in văn bản.  g++ -std=c++17 -O2 lab_checklist.cpp -o demo
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::pair<std::string,std::string>> roe = {
        {"Mục tiêu", "binary của tôi / crackme / được phép bằng văn bản"},
        {"Phạm vi", "offline, read-only hoặc dry-run"},
        {"Lab", "Windows VM + snapshot + x64dbg + DIE"},
        {"Hash", "SHA-256 trước/sau khi phân tích"},
        {"Rollback", "kế hoạch khôi phục nếu có sự cố"},
    };
    std::cout << "RULES OF ENGAGEMENT\n" << std::string(56,'=') << "\n\n";
    int i = 1;
    for (auto& [k, v] : roe) std::cout << i++ << ". " << k << ": " << v << "\n";
    std::cout << "\nChỉ thực hành trong lab thuộc quyền kiểm soát, có RoE.\n";
    return 0;
}
