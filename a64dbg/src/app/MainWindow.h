#pragma once

#include <QMainWindow>

class QDockWidget;
class QPlainTextEdit;
class QTableWidget;

namespace a64dbg {

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);

private:
  void buildMenus();
  void buildDocks();
  void showAbout();

  QPlainTextEdit* m_disasmView = nullptr;
  QPlainTextEdit* m_dumpView = nullptr;
  QPlainTextEdit* m_stackView = nullptr;
  QTableWidget* m_registerView = nullptr;
  QDockWidget* m_stackDock = nullptr;
  QDockWidget* m_dumpDock = nullptr;
};

}  // namespace a64dbg
