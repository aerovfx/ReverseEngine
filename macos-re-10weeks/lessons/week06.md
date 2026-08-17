# Tuần 06 — Runtime & Hooking: Method Swizzling, dylib injection, Frida

## Mục tiêu

- Hiểu và thực hành **method swizzling** trong Objective-C.
- Nắm kỹ thuật **dylib injection** (DYLD_INSERT_LIBRARIES) trên app của chính mình.
- Viết script **Frida** để hook và sửa hành vi runtime.

## Công cụ và dữ liệu

- Xcode CLT, `clang`, Frida.
- App Objective-C do chính bạn viết (để swizzle/inject hợp pháp).

## Lý thuyết và ví dụ

**Method swizzling** — đổi implementation của 2 method tại runtime bằng Objective-C runtime API:
```objc
Method a = class_getInstanceMethod(cls, selA);
Method b = class_getInstanceMethod(cls, selB);
method_exchangeImplementations(a, b);
```
Đây là kỹ thuật nền tảng để hook app (và cũng là cách malware macOS can thiệp vào app khác — nên hiểu để phòng thủ).

**dylib injection** — dùng `DYLD_INSERT_LIBRARIES=path.dylib ./app` để nạp thư viện của mình vào tiến trình app, từ đó hook. **Chỉ dùng trên app của chính bạn** trong lab (macOS có cơ chế chặn `DYLD_INSERT_LIBRARIES` cho app có hardened runtime).

**Frida** — cung cấp `Interceptor`, `ObjC.api` để hook method Objective-C:
```js
var cls = ObjC.classes.MyClass;
Interceptor.attach(cls["-doThing:"].implementation, {
  onEnter(args) { console.log("doThing called"); }
});
```

## Lab từng bước

1. Viết app Objective-C với class `MyClass` có method `doThing:`.
2. Viết category + swizzle `doThing:` với method khác; chạy và quan sát hành vi đổi.
3. Viết dylib đơn giản (in log khi load) và chạy `DYLD_INSERT_LIBRARIES` trên app của bạn.
4. Viết script Frida hook method `doThing:` bằng `ObjC.api`.
5. Ghi chú kỹ thuật nào hợp pháp cho app của mình, kỹ thuật nào cần thận trọng.

## Liên kết code mẫu

Xem [code/week06/README.md](../code/week06/README.md) — `swizzle_demo.m`, `dylib_inject_demo.m`, `frida_objc_hook.js`.

## Thảo luận

- Vì sao method swizzling vừa là công cụ RE vừa là kỹ thuật malware?
- Hardened runtime chặn dylib injection như thế nào?

## Bài tập

- **Cơ bản:** Giải thích method swizzling và khi nào dùng.
- **Nâng cao:** Viết dylib injection vào app của bạn và log thứ tự nạp.
- **Thử thách:** Viết script Frida hook một method Objective-C và thay đổi giá trị trả về.

## Nộp bài và rubric

Nộp demo hook (swizzle + dylib + Frida) + giải thích. Rubric chuẩn 100 điểm.

> **Cảnh báo pháp lý:** method swizzling và dylib injection **chỉ thực hành trên app của chính bạn** trong VM lab. Không dùng để can thiệp app của bên thứ ba, né license, hoặc phát tán. Tuần 07 xét network và persistence.
