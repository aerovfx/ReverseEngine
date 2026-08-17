// dbg_sdk.h — Plugin SDK (C ABI) cho a64dbg.
// Plugin build dưới dạng loadable module (.so/.dylib) và export:
//   void pluginit(dbg_plugin_info* info, const dbg_api* api);
//   void plugstop(void);
//   void plugsetup(void);
#ifndef DBG_SDK_H
#define DBG_SDK_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DBG_SDK_VERSION 1

typedef enum {
  DBG_EVENT_INIT = 0,
  DBG_EVENT_STOP = 1,
  DBG_EVENT_BREAKPOINT = 2,
  DBG_EVENT_EXIT = 3,
} dbg_event_type;

typedef struct {
  dbg_event_type type;
  uint64_t pc;
  uint64_t address;
  char message[256];
} dbg_event;

// API mà debugger cung cấp cho plugin.
typedef struct {
  void (*log)(const char* msg);
  int (*read_memory)(uint64_t addr, void* buf, size_t len);
  int (*write_memory)(uint64_t addr, const void* buf, size_t len);
  int (*read_registers)(uint64_t* regs, size_t count);
  // Plugin gọi để đăng ký callback nhận sự kiện từ debugger.
  void (*set_event_callback)(void (*cb)(const dbg_event*));
} dbg_api;

typedef struct {
  int sdk_version;
  const char* name;
  const char* version;
} dbg_plugin_info;

#ifdef __cplusplus
}
#endif

#endif  // DBG_SDK_H
