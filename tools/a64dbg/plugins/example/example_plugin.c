// example_plugin — plugin mẫu: đăng ký callback và đếm sự kiện.
#include "dbg_sdk.h"

static dbg_api g_api;
static int g_event_count = 0;

static void my_callback(const dbg_event* e) {
  g_event_count++;
  if (e != NULL) {
    g_api.log("example_plugin: event");
  }
}

void pluginit(dbg_plugin_info* info, const dbg_api* api) {
  if (info != NULL) {
    info->sdk_version = DBG_SDK_VERSION;
    info->name = "example_plugin";
    info->version = "1.0";
  }
  if (api != NULL) {
    g_api = *api;
    g_api.set_event_callback(my_callback);
  }
}

void plugstop(void) {
  g_api.log("example_plugin: stop");
}

void plugsetup(void) {
}

// Hàm trợ giúp để test xác minh callback đã chạy.
int get_event_count(void) {
  return g_event_count;
}
