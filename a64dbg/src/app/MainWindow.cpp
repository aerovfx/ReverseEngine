#include "MainWindow.h"

#include "Disassembler.h"

#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QHeaderView>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QTableWidget>

namespace a64dbg {

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowTitle(QStringLiteral("a64dbg — macOS ARM64 debugger"));
  resize(1400, 900);

  buildDocks();
  buildMenus();

  statusBar()->showMessage(
      QStringLiteral("Capstone %1 — not attached").arg(capstoneVersion()));
}

void MainWindow::buildMenus() {
  QMenu* file = menuBar()->addMenu(QStringLiteral("&File"));
  file->addAction(QStringLiteral("&Open…"), this, [] {});
  file->addAction(QStringLiteral("&Attach…"), this, [] {});
  file->addAction(QStringLiteral("&Detach"), this, [] {});
  file->addSeparator();
  file->addAction(QStringLiteral("&Quit"), this, [] { QApplication::quit(); });

  QMenu* debug = menuBar()->addMenu(QStringLiteral("&Debug"));
  debug->addAction(QStringLiteral("&Run / Continue"), this, [] {});
  debug->addAction(QStringLiteral("Step &Into"), this, [] {});
  debug->addAction(QStringLiteral("Step &Over"), this, [] {});
  debug->addAction(QStringLiteral("Step O&ut"), this, [] {});

  QMenu* view = menuBar()->addMenu(QStringLiteral("&View"));
  const auto docks = findChildren<QDockWidget*>();
  for (QDockWidget* d : docks) {
    view->addAction(d->toggleViewAction());
  }

  QMenu* help = menuBar()->addMenu(QStringLiteral("&Help"));
  help->addAction(QStringLiteral("&About"), this, &MainWindow::showAbout);
}

void MainWindow::buildDocks() {
  setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);

  // CPU / disassembly — view chính.
  m_disasmView = new QPlainTextEdit(this);
  m_disasmView->setReadOnly(true);
  m_disasmView->setPlaceholderText(
      QStringLiteral("CPU / disassembly — Phase 2 (Capstone AArch64)"));
  auto* cpu = new QDockWidget(QStringLiteral("CPU"), this);
  cpu->setWidget(m_disasmView);
  addDockWidget(Qt::TopDockWidgetArea, cpu);

  // Registers — bảng placeholder cho AArch64 x0..x30, sp, pc, cpsr.
  m_registerView = new QTableWidget(0, 2, this);
  m_registerView->setHorizontalHeaderLabels(
      {QStringLiteral("Register"), QStringLiteral("Value")});
  m_registerView->horizontalHeader()->setStretchLastSection(true);
  m_registerView->verticalHeader()->setVisible(false);
  const char* regs[] = {"x0",  "x1",  "x2",  "x3",  "x4",  "x5",  "x6",  "x7",
                        "x8",  "x9",  "x10", "x11", "x12", "x13", "x14", "x15",
                        "x16", "x17", "x18", "x19", "x20", "x21", "x22", "x23",
                        "x24", "x25", "x26", "x27", "x28", "x29", "x30", "sp",
                        "pc",  "cpsr"};
  const int n = static_cast<int>(sizeof(regs) / sizeof(regs[0]));
  m_registerView->setRowCount(n);
  for (int i = 0; i < n; ++i) {
    m_registerView->setItem(i, 0,
                            new QTableWidgetItem(QString::fromLatin1(regs[i])));
    m_registerView->setItem(
        i, 1, new QTableWidgetItem(QStringLiteral("0x0000000000000000")));
  }
  auto* reg = new QDockWidget(QStringLiteral("Registers"), this);
  reg->setWidget(m_registerView);
  addDockWidget(Qt::RightDockWidgetArea, reg);

  // Stack — dưới-trái.
  m_stackView = new QPlainTextEdit(this);
  m_stackView->setReadOnly(true);
  m_stackView->setPlaceholderText(QStringLiteral("Stack view — Phase 5"));
  m_stackDock = new QDockWidget(QStringLiteral("Stack"), this);
  m_stackDock->setWidget(m_stackView);
  addDockWidget(Qt::BottomDockWidgetArea, m_stackDock);

  // Dump — dưới-phải (tab hóa với Stack).
  m_dumpView = new QPlainTextEdit(this);
  m_dumpView->setReadOnly(true);
  m_dumpView->setPlaceholderText(
      QStringLiteral("Memory dump (hex + ASCII) — Phase 2"));
  m_dumpDock = new QDockWidget(QStringLiteral("Dump"), this);
  m_dumpDock->setWidget(m_dumpView);
  addDockWidget(Qt::BottomDockWidgetArea, m_dumpDock);

  tabifyDockWidget(m_stackDock, m_dumpDock);
  m_stackDock->raise();
}

void MainWindow::showAbout() {
  QMessageBox::about(
      this, QStringLiteral("About a64dbg"),
      QStringLiteral("a64dbg — macOS ARM64 debugger (x64dbg-like)\nCapstone: %1")
          .arg(capstoneVersion()));
}

}  // namespace a64dbg
