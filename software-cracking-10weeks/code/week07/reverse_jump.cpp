// 07_reverse_jump.cpp — Mô phỏng đảo nhảy điều kiện để quan sát nhánh ẩn
// Mục tiêu: đổi JE<->JNE (hoặc lật ZF) làm luồng rẽ sang nhánh 'thành công'.
// An toàn: thuần tính toán; chỉ là kỹ thuật HIỂU luồng trên crackme của bạn.  g++ -std=c++17 -O2 reverse_jump.cpp -o demo
#include <iostream>
#include <string>

const std::string CORRECT = "RE-2024";

// Mô phỏng: CMP input, correct; JE success. invert=true đảo quyết định.
const char* check(const std::string& input, bool invert) {
    bool equal = (input == CORRECT);
    if (invert) equal = !equal;   // đảo nhảy: JE -> JNE
    return equal ? "FLAG{bypass_ok}" : "try harder";
}

int main() {
    std::cout << "Serial đúng (ẩn): " << CORRECT << "\n\n";
    for (const std::string& attempt : {std::string("sai-pass"), CORRECT}) {
        std::cout << "Input '" << attempt << "':\n";
        std::cout << "   bình thường : " << check(attempt, false) << "\n";
        std::cout << "   đảo nhảy    : " << check(attempt, true) << "\n";
        std::cout << std::string(40, '-') << "\n";
    }
    std::cout << "Kết luận: đảo nhảy làm luồng rẽ sang nhánh 'thành công' dù input sai.\n";
    std::cout << "Lưu ý: chỉ trên crackme/binary của bạn, không dùng cho phần mềm thương mại.\n";
    return 0;
}
