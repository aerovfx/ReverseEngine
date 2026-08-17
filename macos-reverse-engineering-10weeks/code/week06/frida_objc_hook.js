// ============================================================
// frida_objc_hook.js — Hook method Objective-C bằng Frida
// Mục tiêu: hook [MyClass doThing:] và log tham số / đổi kết quả.
// Cách dùng: frida -n YourApp -l frida_objc_hook.js
// An toàn: chỉ dùng trên app của chính bạn / được cấp phép.
// ============================================================
if (ObjC.available) {
  var MyClass = ObjC.classes.MyClass;
  if (MyClass) {
    var doThing = MyClass["- doThing:"];
    if (doThing) {
      Interceptor.attach(doThing.implementation, {
        onEnter(args) {
          var self = new ObjC.Object(args[0]);
          var arg = new ObjC.Object(args[2]);
          console.log("[hook] -[MyClass doThing:] gọi trên " + self + " arg=" + arg);
        },
        onLeave(retval) {
          console.log("[hook] trả về " + retval);
        },
      });
      console.log("[*] Đã hook -[MyClass doThing:]");
    }
  } else {
    console.log("[!] Không thấy MyClass");
  }
}
