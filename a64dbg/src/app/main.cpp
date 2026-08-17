#include "Disassembler.h"
#include "Log.h"
#include "MainWindow.h"
#include "Theme.h"
#include "Version.h"

#include <QApplication>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QApplication::setApplicationName(QStringLiteral("a64dbg"));
  QApplication::setApplicationVersion(QStringLiteral(A64DBG_VERSION_STRING));
  QApplication::setOrganizationName(QStringLiteral("ReverseEngine"));

  a64dbg::applyDarkTheme(app);
  a64dbg::logInfo(std::string("a64dbg ") + A64DBG_VERSION_STRING +
                  " starting; capstone " + a64dbg::capstoneVersion());

  a64dbg::MainWindow window;
  window.show();
  return app.exec();
}
