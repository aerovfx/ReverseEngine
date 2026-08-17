# Plugin mẫu build dạng loadable module cho dlopen.
add_library(example_plugin MODULE example_plugin.c)
target_include_directories(example_plugin PRIVATE ${CMAKE_SOURCE_DIR}/include)
set_target_properties(example_plugin PROPERTIES
  OUTPUT_NAME "example_plugin"
  PREFIX ""
)
