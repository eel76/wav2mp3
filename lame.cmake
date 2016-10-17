include(ExternalProject)

ExternalProject_Add(lame-build
  URL http://downloads.sourceforge.net/project/lame/lame/3.99/lame-3.99.5.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/lame-3.99.5
  PATCH_COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-lame.txt" "${CMAKE_CURRENT_BINARY_DIR}/lame-3.99.5/src/lame-build/CMakeLists.txt"
  COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/lame-3.99.5/src/lame-build/configMS.h" "${CMAKE_CURRENT_BINARY_DIR}/lame-3.99.5/src/lame-build/config.h"
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(lame-build source_dir)
ExternalProject_Get_Property(lame-build binary_dir)

add_library(lame STATIC IMPORTED)

file(MAKE_DIRECTORY "${source_dir}/include")
set_property(TARGET lame PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${source_dir}/include")
set_property(TARGET lame PROPERTY IMPORTED_LOCATION "${binary_dir}/${CMAKE_CFG_INTDIR}/libmp3lame-static.lib")

add_dependencies(lame lame-build)
