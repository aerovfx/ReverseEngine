// ============================================================
// frida_hook_template.js — Template hook hàm bằng Frida
// Mục tiêu: hook strcmp (hoặc hàm khác) và log tham số.
// Cách dùng: frida -n <AppName> -l frida_hook_template.js
// An toàn: chỉ dùng trên app của chính bạn / được cấp phép.
// ============================================================
if (ObjC.available) {
  console.log("[*] Objective-C runtime available");
}

// Hook strcmp trong libSystem
var strcmp = Module.findExportByName(null, "strcmp");
if (strcmp) {
  Interceptor.attach(strcmp, {
    onEnter(args) {
      this.a = args[0];
      this.b = args[1];
      try {
        console.log(
          "[strcmp] " +
            this.a.readCString() +
            " <=> " +
            this.b.readCString()
        );
      } catch (e) {
        console.log("[strcmp] (không đọc được chuỗi)");
      }
    },
    onLeave(retval) {
      console.log("[strcmp] -> " + retval);
    },
  });
  console.log("[*] Đã hook strcmp");
} else {
  console.log("[!] Không tìm thấy strcmp");
}
