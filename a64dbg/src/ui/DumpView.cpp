#include "DumpView.h"

#include <QFontDatabase>

namespace a64dbg {

DumpView::DumpView(QWidget* parent) : QPlainTextEdit(parent) {
  setReadOnly(true);
  setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

void DumpView::show(std::uint64_t addr, const std::vector<std::uint8_t>& bytes) {
  constexpr std::size_t kPerLine = 16;
  QString text;
  for (std::size_t off = 0; off < bytes.size(); off += kPerLine) {
    QString line = QString::asprintf("%016llx  ", static_cast<unsigned long long>(addr + off));
    QString ascii;
    for (std::size_t i = 0; i < kPerLine; ++i) {
      if (off + i < bytes.size()) {
        const std::uint8_t b = bytes[off + i];
        line += QString::asprintf("%02x ", b);
        ascii += (b >= 0x20 && b < 0x7f) ? QChar(b) : QChar('.');
      } else {
        line += QStringLiteral("   ");
      }
      if (i == 7) {
        line += ' ';
      }
    }
    text += line + QStringLiteral("  ") + ascii + QLatin1Char('\n');
  }
  setPlainText(text);
}

}  // namespace a64dbg
