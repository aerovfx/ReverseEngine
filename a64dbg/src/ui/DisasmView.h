#pragma once

#include "Disassembler.h"

#include <QPlainTextEdit>

#include <cstdint>
#include <vector>

namespace a64dbg {

// View hiển thị disassembly (Phase 2: bản text monospace, đánh dấu PC bằng "→").
class DisasmView : public QPlainTextEdit {
  Q_OBJECT

public:
  explicit DisasmView(QWidget* parent = nullptr);

  void show(const std::vector<Instruction>& insns, std::uint64_t highlightPc);
};

}  // namespace a64dbg
