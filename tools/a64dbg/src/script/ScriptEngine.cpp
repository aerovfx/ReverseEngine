#include "ScriptEngine.h"

#include <QJSValue>

#include <cstdio>

namespace a64dbg::script {

ScriptApi::ScriptApi(MemoryReader reader, QObject* parent)
    : QObject(parent), m_reader(std::move(reader)) {}

void ScriptApi::log(const QString& msg) {
  std::fprintf(stderr, "[script] %s\n", msg.toUtf8().constData());
}

QString ScriptApi::readMemory(const QString& addrHex, int len) {
  bool ok = false;
  const std::uint64_t addr = addrHex.toULongLong(&ok, 16);
  if (!ok || len <= 0 || len > 4096 || !m_reader) {
    return QString();
  }
  std::vector<std::uint8_t> buf(static_cast<std::size_t>(len));
  if (!m_reader(addr, buf.data(), buf.size())) {
    return QString();
  }
  QString s;
  for (const auto b : buf) {
    s += QString::asprintf("%02x", b);
  }
  return s;
}

ScriptEngine::ScriptEngine(ScriptApi::MemoryReader reader) {
  m_api = new ScriptApi(std::move(reader), &m_engine);
  m_engine.globalObject().setProperty(QStringLiteral("debug"), m_engine.newQObject(m_api));
}

bool ScriptEngine::eval(const std::string& code, std::string& result) {
  const QJSValue v = m_engine.evaluate(QString::fromStdString(code));
  if (v.isError()) {
    result = v.toString().toStdString();
    return false;
  }
  result = v.toString().toStdString();
  return true;
}

}  // namespace a64dbg::script
