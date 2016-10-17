ExternalProject_Add(gtest-build
  GIT_REPOSITORY https://github.com/google/googletest.git
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/googletest
  UPDATE_COMMAND ""
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(gtest-build source_dir)
ExternalProject_Get_Property(gtest-build binary_dir)

add_library(gtest STATIC IMPORTED)

file(MAKE_DIRECTORY "${source_dir}/googletest/include")
set_property(TARGET gtest PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${source_dir}/googletest/include")
set_property(TARGET gtest PROPERTY IMPORTED_LOCATION "${binary_dir}/googlemock/gtest/${CMAKE_CFG_INTDIR}/gtest.lib")

if (MSVC)
  set_property(TARGET gtest PROPERTY INTERFACE_COMPILE_OPTIONS $<$<CONFIG:Debug>:/MTd> $<$<NOT:$<CONFIG:Debug>>:/MT>)
endif()

add_dependencies(gtest gtest-build)
