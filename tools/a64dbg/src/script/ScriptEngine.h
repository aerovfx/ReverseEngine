#pragma once

#include <QJSEngine>
#include <QObject>

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace a64dbg::script {

// Đối tượng `debug` lộ cho JS.
class ScriptApi : public QObject {
  Q_OBJECT
public:
  using MemoryReader = std::function<bool(std::uint64_t, void*, std::size_t)>;

  explicit ScriptApi(MemoryReader reader, QObject* parent = nullptr);

  Q_INVOKABLE void log(const QString& msg);
  Q_INVOKABLE QString readMemory(const QString& addrHex, int len);

private:
  MemoryReader m_reader;
};

// Script console: chạy JS với đối tượng `debug`.
class ScriptEngine {
public:
  explicit ScriptEngine(ScriptApi::MemoryReader reader);

  bool eval(const std::string& code, std::string& result);

private:
  QJSEngine m_engine;
  ScriptApi* m_api = nullptr;
};

}  // namespace a64dbg::script
