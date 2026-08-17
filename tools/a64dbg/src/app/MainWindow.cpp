#include "MainWindow.h"

#include "DebuggerController.h"
#include "DisasmView.h"
#include "Disassembler.h"
#include "DumpView.h"

#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QHeaderView>
#include <QInputDialog>
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

  m_controller = new DebuggerController(this);
  connect(m_controller, &DebuggerController::stopped, this, &MainWindow::onStopped);
  connect(m_controller, &DebuggerController::running, this, &MainWindow::onRunning);

  buildDocks();
  buildMenus();

  statusBar()->showMessage(
      QStringLiteral("Capstone %1 — not attached").arg(capstoneVersion()));
}

void MainWindow::buildMenus() {
  QMenu* file = menuBar()->addMenu(QStringLiteral("&File"));
  file->addAction(QStringLiteral("&Attach…"), this, &MainWindow::onAttach);
  file->addAction(QStringLiteral("&Detach"), this, &MainWindow::onDetach);
  file->addSeparator();
  file->addAction(QStringLiteral("&Quit"), this, [] { QApplication::quit(); });

  QMenu* debug = menuBar()->addMenu(QStringLiteral("&Debug"));
  debug->addAction(QStringLiteral("&Run / Continue"), this, &MainWindow::onContinue);
  debug->addAction(QStringLiteral("Step &Into"), this, &MainWindow::onStepInto);

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

  // CPU / disassembly.
  m_disasmView = new DisasmView(this);
  m_disasmView->setPlaceholderText(QStringLiteral("Attach để xem disassembly (Phase 2)"));
  auto* cpu = new QDockWidget(QStringLiteral("CPU"), this);
  cpu->setWidget(m_disasmView);
  addDockWidget(Qt::TopDockWidgetArea, cpu);

  // Registers — bảng AArch64 x0..x30, sp, pc, cpsr.
  m_registerView = new QTableWidget(0, 2, this);
  m_registerView->setHorizontalHeaderLabels(
      {QStringLiteral("Register"), QStringLiteral("Value")});
  m_registerView->horizontalHeader()->setStretchLastSection(true);
  m_registerView->verticalHeader()->setVisible(false);
  m_registerView->setRowCount(core::kRegisterCount);
  for (int i = 0; i < core::kRegisterCount; ++i) {
    m_registerView->setItem(i, 0,
                            new QTableWidgetItem(QString::fromLatin1(core::kRegisterNames[i])));
    m_registerView->setItem(i, 1,
                            new QTableWidgetItem(QStringLiteral("0x0000000000000000")));
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
  m_dumpView = new DumpView(this);
  m_dumpView->setPlaceholderText(QStringLiteral("Memory dump (hex + ASCII) — Phase 2"));
  m_dumpDock = new QDockWidget(QStringLiteral("Dump"), this);
  m_dumpDock->setWidget(m_dumpView);
  addDockWidget(Qt::BottomDockWidgetArea, m_dumpDock);

  tabifyDockWidget(m_stackDock, m_dumpDock);
  m_stackDock->raise();
}

void MainWindow::onAttach() {
  bool ok = false;
  const int pid = QInputDialog::getInt(this, QStringLiteral("Attach"),
                                       QStringLiteral("PID:"), 0, 0, 2147483647, 1, &ok);
  if (!ok) {
    return;
  }
  if (m_controller->attach(pid)) {
    statusBar()->showMessage(QStringLiteral("attached pid %1").arg(pid));
  } else {
    statusBar()->showMessage(
        QStringLiteral("attach pid %1 thất bại (cần entitlement/quyền)").arg(pid));
  }
}

void MainWindow::onDetach() {
  m_controller->detach();
  statusBar()->showMessage(QStringLiteral("detached"));
}

void MainWindow::onContinue() {
  m_controller->runContinue();
}

void MainWindow::onStepInto() {
  m_controller->stepInto();
}

void MainWindow::onRunning() {
  statusBar()->showMessage(QStringLiteral("running…"));
}

void MainWindow::onStopped() {
  core::RegisterState rs;
  std::uint64_t pc = 0;
  std::uint64_t sp = 0;
  if (m_controller->readRegisters(rs)) {
    updateRegisters(rs);
    pc = rs.r[static_cast<int>(core::Reg::PC)];
    sp = rs.r[static_cast<int>(core::Reg::SP)];
  }

  std::vector<Instruction> insns;
  if (m_controller->disassembleAt(pc, 40, insns)) {
    m_disasmView->show(insns, pc);
  }

  std::vector<std::uint8_t> mem;
  if (m_controller->readMemory(sp, 256, mem)) {
    m_dumpView->show(sp, mem);
  }

  statusBar()->showMessage(
      QString::asprintf("stopped at 0x%llx", static_cast<unsigned long long>(pc)));
}

void MainWindow::updateRegisters(const core::RegisterState& rs) {
  for (int i = 0; i < m_registerView->rowCount(); ++i) {
    QTableWidgetItem* item = m_registerView->item(i, 1);
    if (item == nullptr) {
      item = new QTableWidgetItem();
      m_registerView->setItem(i, 1, item);
    }
    item->setText(QString::asprintf("0x%016llx", static_cast<unsigned long long>(rs.r[i])));
  }
}

void MainWindow::showAbout() {
  QMessageBox::about(
      this, QStringLiteral("About a64dbg"),
      QStringLiteral("a64dbg — macOS ARM64 debugger (x64dbg-like)\nCapstone: %1")
          .arg(capstoneVersion()));
}

}  // namespace a64dbg
