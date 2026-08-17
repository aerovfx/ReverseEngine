// Phase 6: scripting qua QJSEngine — đối tượng `debug` (log, readMemory).
#include "ScriptEngine.h"

#include <QCoreApplication>

#include <cstdio>
#include <cstring>

int main(int argc, char** argv) {
  QCoreApplication app(argc, argv);

  // Stub reader: trả 4 byte cố định.
  static const unsigned char fake[4] = {0xde, 0xad, 0xbe, 0xef};
  a64dbg::script::ScriptEngine engine(
      [](std::uint64_t, void* buf, std::size_t len) {
        std::memcpy(buf, fake, len < 4 ? len : 4);
        return true;
      });

  std::string result;

  if (!engine.eval("1 + 2", result)) {
    std::fprintf(stderr, "eval 1+2 lỗi: %s\n", result.c_str());
    return 1;
  }
  std::printf("1+2 = %s\n", result.c_str());
  if (result != "3") {
    std::fprintf(stderr, "kết quả sai: %s\n", result.c_str());
    return 1;
  }

  if (!engine.eval("debug.readMemory('0x1000', 4)", result)) {
    std::fprintf(stderr, "eval readMemory lỗi: %s\n", result.c_str());
    return 1;
  }
  std::printf("readMemory = %s\n", result.c_str());
  if (result != "deadbeef") {
    std::fprintf(stderr, "readMemory sai: %s\n", result.c_str());
    return 1;
  }

  engine.eval("debug.log('hello from script')", result);

  std::printf("TEST PASSED\n");
  return 0;
}
