find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(a64dbg MANUAL_FINALIZATION
  main.cpp
  MainWindow.h
  MainWindow.cpp
  Theme.h
  Theme.cpp
)

target_link_libraries(a64dbg PRIVATE
  Qt6::Widgets
  a64dbg_common
  a64dbg_model
  a64dbg_disasm
  a64dbg_core
)

set_target_properties(a64dbg PROPERTIES
  MACOSX_BUNDLE TRUE
  MACOSX_BUNDLE_BUNDLE_NAME "a64dbg"
  MACOSX_BUNDLE_GUI_IDENTIFIER "com.reverseengine.a64dbg"
  MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
  MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
)

a64dbg_enable_warnings(a64dbg)

qt_finalize_executable(a64dbg)

# Ad-hoc sign với entitlement debugger (cần khi attach tiến trình khác — Phase 1).
# Chạy: cmake --build <dir> --target sign-a64dbg
add_custom_target(sign-a64dbg
  COMMAND codesign --force --sign - --entitlements
          "${CMAKE_SOURCE_DIR}/entitlements/debugger.entitlements"
          "$<TARGET_BUNDLE_DIR:a64dbg>"
  DEPENDS a64dbg
  COMMENT "Ad-hoc code-sign a64dbg.app với com.apple.security.cs.debugger"
  VERBATIM
)
