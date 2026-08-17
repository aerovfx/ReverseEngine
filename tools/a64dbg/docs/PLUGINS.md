# Viết plugin cho a64dbg

Plugin là loadable module (`.so`/`.dylib`) nạp bằng `dlopen`, giao tiếp qua C ABI trong
[`include/dbg_sdk.h`](../include/dbg_sdk.h). Không cần link thư viện nào của a64dbg — chỉ include header.

## Contract

Plugin phải export 3 hàm:

```c
void pluginit(dbg_plugin_info* info, const dbg_api* api);
void plugstop(void);
void plugsetup(void);
```

- `pluginit` được gọi khi nạp. Điền `info` (name/version) và **giữ lại `api`** để gọi các hàm debugger.
- `plugstop` được gọi khi gỡ plugin.
- `plugsetup` được gọi khi bắt đầu một phiên debug (dành cho khởi tạo session).

## API (`dbg_api`)

| Hàm | Ý nghĩa |
|---|---|
| `void log(const char* msg)` | Ghi log (stderr) |
| `int read_memory(uint64_t addr, void* buf, size_t len)` | Đọc bộ nhớ target (1 = ok) |
| `int write_memory(uint64_t addr, const void* buf, size_t len)` | Ghi bộ nhớ target |
| `int read_registers(uint64_t* regs, size_t count)` | Đọc thanh ghi (34 regs, xem `RegisterSet.h`) |
| `void set_event_callback(void (*cb)(const dbg_event*))` | Đăng ký callback nhận sự kiện |

## Sự kiện (`dbg_event`)

| `type` | Khi nào |
|---|---|
| `DBG_EVENT_INIT` | Bắt đầu phiên debug |
| `DBG_EVENT_STOP` | Target dừng (step/exception) |
| `DBG_EVENT_BREAKPOINT` | Trúng breakpoint |
| `DBG_EVENT_EXIT` | Kết thúc phiên |

## Ví dụ

```c
#include "dbg_sdk.h"

static dbg_api g_api;
static int g_events = 0;

static void on_event(const dbg_event* e) {
  g_events++;
  g_api.log("event!");
}

void pluginit(dbg_plugin_info* info, const dbg_api* api) {
  info->sdk_version = DBG_SDK_VERSION;
  info->name = "my_plugin";
  info->version = "1.0";
  g_api = *api;
  g_api.set_event_callback(on_event);
}

void plugstop(void) {}
void plugsetup(void) {}
```

Build (CMake):

```cmake
add_library(my_plugin MODULE my_plugin.c)
target_include_directories(my_plugin PRIVATE <a64dbg>/include)
```

## Nạp plugin

Từ code: `PluginHost::load(path)` (hoặc `DebuggerController::loadPlugin(path)`).
Từ GUI: sẽ thêm menu "Plugin" ở bản sau.
