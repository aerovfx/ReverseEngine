// 09_dotnet.cpp — Mô phỏng metadata .NET (assembly/class/method) để RE
// Mục tiêu: hiểu cấu trúc .NET dễ reverse (IL + metadata) và khái niệm deobfuscation.
// An toàn: thuần tính toán — mô phỏng metadata bằng struct.  g++ -std=c++17 -O2 dotnet.cpp -o demo
#include <iostream>
#include <string>
#include <vector>

struct Method { std::string name, il; };
struct Class   { std::string name; std::vector<Method> methods; };

int main() {
    Class license {
        "LicenseChecker",
        {{"Check", "ldarg.0; call GetSerial; brtrue OK"}, {"GetSerial", "ldstr \"KEY-123\"; ret"}}
    };
    std::cout << ".NET METADATA (đơn giản hoá)\n" << std::string(56,'=') << "\n\n";
    std::cout << "Class: " << license.name << "\n";
    for (auto& m : license.methods)
        std::cout << "  Method " << m.name << " : " << m.il << "\n";

    std::cout << "\nKết luận: .NET giữ IL + metadata nên dễ decompile (dnSpy/ILSpy);\n";
    std::cout << "deobfuscation (de4dot) dùng để khôi phục tên/điều khiển luồng.\n";
    return 0;
}
