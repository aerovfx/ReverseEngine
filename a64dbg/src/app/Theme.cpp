#include "Theme.h"

#include <QApplication>
#include <QPalette>
#include <QStyleFactory>

namespace a64dbg {

void applyDarkTheme(QApplication& app) {
  app.setStyle(QStyleFactory::create(QStringLiteral("Fusion")));

  const QColor window(0x1e, 0x1e, 0x1e);
  const QColor base(0x18, 0x18, 0x18);
  const QColor text(0xdc, 0xdc, 0xdc);
  const QColor highlight(0x2d, 0x6c, 0xdf);
  const QColor disabled(0x80, 0x80, 0x80);

  QPalette p;
  p.setColor(QPalette::Window, window);
  p.setColor(QPalette::WindowText, text);
  p.setColor(QPalette::Base, base);
  p.setColor(QPalette::AlternateBase, window);
  p.setColor(QPalette::ToolTipBase, base);
  p.setColor(QPalette::ToolTipText, text);
  p.setColor(QPalette::Text, text);
  p.setColor(QPalette::Button, window);
  p.setColor(QPalette::ButtonText, text);
  p.setColor(QPalette::BrightText, Qt::red);
  p.setColor(QPalette::Link, highlight);
  p.setColor(QPalette::Highlight, highlight);
  p.setColor(QPalette::HighlightedText, Qt::white);
  p.setColor(QPalette::Disabled, QPalette::Text, disabled);
  p.setColor(QPalette::Disabled, QPalette::WindowText, disabled);
  p.setColor(QPalette::Disabled, QPalette::ButtonText, disabled);
  app.setPalette(p);
}

}  // namespace a64dbg
