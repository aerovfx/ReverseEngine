#pragma once

#include <QPlainTextEdit>

#include <cstdint>
#include <vector>

namespace a64dbg {

// View hiển thị memory dump (hex + ASCII), 16 byte/dòng.
class DumpView : public QPlainTextEdit {
  Q_OBJECT

public:
  explicit DumpView(QWidget* parent = nullptr);

  void show(std::uint64_t addr, const std::vector<std::uint8_t>& bytes);
};

}  // namespace a64dbg
