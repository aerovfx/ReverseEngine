// 01_lab_checklist.cpp — Checklist môi trường lab cracking an toàn
// Mục tiêu: sinh danh sách kiểm tra (RoE + lab cô lập) trước khi phân tích binary.
// An toàn: thuần tính toán, chỉ in văn bản.  g++ -std=c++17 -O2 lab_checklist.cpp -o demo
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

struct Item { std::string muc, ghi_chu; };

int main() {
    std::vector<Item> ck = {
        {"Mục tiêu hợp pháp", "binary của tôi / crackme công khai / được phép bằng văn bản"},
        {"Cô lập", "chạy trong VM, có snapshot 'Base'"},
        {"Mạng", "tắt mạng VM hoặc dùng mạng giả lập"},
        {"Hash", "ghi SHA-256 của binary trước khi chạy"},
        {"Backup", "sao lưu binary gốc trước khi vá"},
        {"RoE", "viết Rules of Engagement (phạm vi + rollback)"},
    };
    std::cout << "LAB CHECKLIST — cracking an toàn\n" << std::string(60, '=') << "\n\n";
    int i = 1;
    for (const auto& it : ck)
        std::cout << "[ ] " << i++ << ". " << it.muc << ": " << it.ghi_chu << "\n";
    std::cout << "\nNếu bất kỳ mục nào chưa đạt, KHÔNG chạy mẫu.\n";
    return 0;
}
