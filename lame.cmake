ExternalProject_Add(lame
  URL http://downloads.sourceforge.net/project/lame/lame/3.99/lame-3.99.5.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/lame-3.99.5
  PATCH_COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/lame-CMakeLists.txt" "${CMAKE_CURRENT_BINARY_DIR}/lame-3.99.5/src/lame/CMakeLists.txt"
  COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/lame-3.99.5/src/lame/configMS.h" "${CMAKE_CURRENT_BINARY_DIR}/lame-3.99.5/src/lame/config.h"
  INSTALL_COMMAND ""
)

set_target_properties(lame PROPERTIES FOLDER "ExternalProjects")

ExternalProject_Get_Property(lame source_dir)
ExternalProject_Get_Property(lame binary_dir)

add_library(libmp3lame-static STATIC IMPORTED)
set_property(TARGET libmp3lame-static PROPERTY IMPORTED_LOCATION "${binary_dir}/${CMAKE_CFG_INTDIR}/libmp3lame-static.lib")
target_include_directories(libmp3lame-static INTERFACE "${source_dir}/include")

add_dependencies(libmp3lame-static lame)
