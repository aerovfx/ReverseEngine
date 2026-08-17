// ============================================================
// frida_license_hook.js — Hook hàm license bằng Frida
// Mục tiêu: hook check_license để luôn trả về hợp lệ (trên app của bạn).
// Cách dùng: frida -n license -l frida_license_hook.js
// An toàn: CHỈ dùng trên app do chính bạn viết, trong VM lab.
//          CẤM dùng để patch app của bên thứ ba (vi phạm bản quyền/DMCA).
// ============================================================
var check = Module.findExportByName(null, "check_license");
if (!check) {
  // binary C không export symbol; tìm qua pattern là bài tập nâng cao
  check = Module.findBaseAddress("license").add(0x0);
}
if (check) {
  Interceptor.replace(check, new NativeCallback(function (key) {
    console.log("[hook] check_license(" + (key ? key.readCString() : "(null)") + ")");
    return 1; // luôn trả "hợp lệ" — chỉ trên app của bạn
  }, "int", ["pointer"]));
  console.log("[*] Đã hook check_license");
} else {
  console.log("[!] Không tìm thấy check_license");
}
