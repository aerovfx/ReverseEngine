// 05_winapi_gui.cpp — Mô phỏng chương trình GUI + Windows API (MessageBox/GetWindowText)
// Mục tiêu: hiểu luồng GUI kiểm tra serial và cách bắt API bằng breakpoint.
// An toàn: thuần tính toán, không gọi Windows API thật.  g++ -std=c++17 -O2 winapi_gui.cpp -o demo
#include <iostream>
#include <string>

const std::string SERIAL = "SR-12345";
std::string get_window_text(const std::string& in) { std::cout << "  [GetWindowTextA] " << in << "\n"; return in; }
void message_box(const std::string& t) { std::cout << "  [MessageBoxA] " << t << "\n"; }

int main(int argc, char** argv) {
    std::string input = (argc > 1) ? argv[1] : "sai";
    std::string s = get_window_text(input);
    bool ok = (s == SERIAL);                 // CMP + JE
    message_box(ok ? "Registered!" : "Invalid serial");
    std::cout << "\nKết luận: BP trên GetWindowTextA/MessageBoxA rồi lần ngược tới CMP.\n";
    return 0;
}
