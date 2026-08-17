// 10_report_builder.cpp — Khung báo cáo RE + checklist trước khi kết thúc
// Mục tiêu: tổng hợp phát hiện RE (Mach-O, hook, license) + checklist an toàn.
// An toàn: thuần tính toán, chỉ in văn bản.  g++ -std=c++17 -O2 report_builder.cpp -o demo
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::cout << "BÁO CÁO RE macOS — khung\n" << std::string(56,'=') << "\n\n";
    std::cout << "1. Mach-O: kiến trúc + load commands\n";
    std::cout << "2. Static: class-dump + strings + codesign\n";
    std::cout << "3. Dynamic: LLDB/Frida + unified log\n";
    std::cout << "4. Cơ chế license: luồng kiểm tra + hook\n";
    std::cout << "5. Đề xuất phòng thủ: server-side check, anti-tamper\n";

    std::cout << "\nChecklist an toàn:\n";
    std::vector<std::string> ck = {"VM cô lập", "snapshot", "hash trước/sau", "ẩn danh dữ liệu"};
    for (auto& c : ck) std::cout << "  [ ] " << c << "\n";
    return 0;
}
