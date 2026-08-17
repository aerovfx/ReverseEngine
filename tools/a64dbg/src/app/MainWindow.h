#pragma once

#include "RegisterSet.h"

#include <QMainWindow>

class QDockWidget;
class QPlainTextEdit;
class QTableWidget;

namespace a64dbg {

class DebuggerController;
class DisasmView;
class DumpView;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);

private slots:
  void onAttach();
  void onDetach();
  void onContinue();
  void onStepInto();
  void onStopped();
  void onRunning();

private:
  void buildMenus();
  void buildDocks();
  void showAbout();
  void updateRegisters(const core::RegisterState& rs);

  DebuggerController* m_controller = nullptr;
  DisasmView* m_disasmView = nullptr;
  DumpView* m_dumpView = nullptr;
  QPlainTextEdit* m_stackView = nullptr;
  QTableWidget* m_registerView = nullptr;
  QDockWidget* m_stackDock = nullptr;
  QDockWidget* m_dumpDock = nullptr;
};

}  // namespace a64dbg
