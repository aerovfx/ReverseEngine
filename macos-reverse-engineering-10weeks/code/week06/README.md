---
layout: course
title: "Readme"
permalink: /macos-reverse-engineering-10weeks/code/week06/README.html
---

# Tuần 06 — Code mẫu

## `swizzle_demo.m`
Demo method swizzling Objective-C.

```bash
clang -framework Foundation swizzle_demo.m -o /tmp/swizzle && /tmp/swizzle
```

## `dylib_inject_demo.m`
Demo dylib injection bằng `DYLD_INSERT_LIBRARIES` trên app của bạn.

```bash
clang -dynamiclib dylib_inject_demo.m -o /tmp/inject.dylib
DYLD_INSERT_LIBRARIES=/tmp/inject.dylib /tmp/hello
```

## `frida_objc_hook.js`
Hook method Objective-C bằng Frida.

```bash
frida -n <YourApp> -l frida_objc_hook.js
```

**An toàn:** chỉ dùng trên app do chính bạn viết, trong VM lab.
