// 09_winapi_sim.cpp — Mô phỏng luồng chương trình GUI kiểm tra serial
// Mục tiêu: GetWindowText (đọc input) + MessageBox (hiển thị) + lệnh so sánh serial.
// An toàn: thuần tính toán, không gọi Windows API thật.  g++ -std=c++17 -O2 winapi_sim.cpp -o demo
#include <iostream>
#include <string>

const std::string CORRECT = "RE-2024";

std::string get_window_text(const std::string& input) {   // mô phỏng GetWindowTextA
    std::cout << "  [GetWindowTextA] đọc input = " << input << "\n";
    return input;
}
void message_box(const std::string& t) {                   // mô phỏng MessageBoxA
    std::cout << "  [MessageBoxA] hiển thị: " << t << "\n";
}

int main(int argc, char** argv) {
    std::string entered = (argc > 1) ? argv[1] : "sai-pass";
    std::string text = get_window_text(entered);
    bool ok = (text == CORRECT);                            // tương ứng CMP + JE
    message_box(ok ? "Correct!" : "Invalid serial");
    std::cout << "\nKết quả: " << (ok ? "ĐÚNG" : "SAI") << " (serial đúng là " << CORRECT << ")\n";
    std::cout << "Khi crack GUI: đặt BP trên GetWindowText/MessageBox rồi lần ngược tới CMP.\n";
    return 0;
}
