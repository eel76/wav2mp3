include(ExternalProject)

ExternalProject_Add(gtest-build
  GIT_REPOSITORY https://github.com/google/googletest.git
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/googletest
  UPDATE_COMMAND ""
  CMAKE_ARGS -Dgtest_force_shared_crt=ON
  UPDATE_COMMAND ""
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(gtest-build source_dir)
ExternalProject_Get_Property(gtest-build binary_dir)

add_library(gtest STATIC IMPORTED)

file(MAKE_DIRECTORY "${source_dir}/googletest/include")
set_property(TARGET gtest PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${source_dir}/googletest/include")
set_property(TARGET gtest PROPERTY IMPORTED_LOCATION "${binary_dir}/googlemock/gtest/${CMAKE_CFG_INTDIR}/gtest.lib")

add_dependencies(gtest gtest-build)
