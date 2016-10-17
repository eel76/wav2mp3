ExternalProject_Add(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/googletest
  UPDATE_COMMAND ""
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(googletest source_dir)
ExternalProject_Get_Property(googletest binary_dir)

add_library(googletest-static STATIC IMPORTED)
set_property(TARGET googletest-static PROPERTY IMPORTED_LOCATION "${binary_dir}/googlemock/gtest/${CMAKE_CFG_INTDIR}/gtest.lib")
set_property(TARGET googletest-static PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${source_dir}/googletest/include")

if (MSVC)
  set_property(TARGET googletest-static PROPERTY INTERFACE_COMPILE_OPTIONS $<$<CONFIG:Debug>:/MTd> $<$<NOT:$<CONFIG:Debug>>:/MT>)
endif()

add_dependencies(googletest-static googletest)
