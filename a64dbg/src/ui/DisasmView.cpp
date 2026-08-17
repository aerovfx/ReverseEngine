#include "DisasmView.h"

#include <QFontDatabase>

namespace a64dbg {
namespace {

QString bytesToHex(const Instruction& ins) {
  QString s;
  for (std::uint8_t i = 0; i < ins.size; ++i) {
    s += QString::asprintf("%02x", ins.bytes[i]);
    if (i + 1 < ins.size) {
      s += ' ';
    }
  }
  return s.leftJustified(12, ' ');
}

}  // namespace

DisasmView::DisasmView(QWidget* parent) : QPlainTextEdit(parent) {
  setReadOnly(true);
  setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

void DisasmView::show(const std::vector<Instruction>& insns, std::uint64_t highlightPc) {
  QString text;
  for (const auto& ins : insns) {
    const bool isPc = (ins.address == highlightPc);
    text += QString::asprintf("%s %012llx  %s  %-8s %s\n",
                              isPc ? "→" : " ",
                              static_cast<unsigned long long>(ins.address),
                              bytesToHex(ins).toUtf8().constData(),
                              ins.mnemonic.c_str(), ins.operands.c_str());
  }
  setPlainText(text);
}

}  // namespace a64dbg
