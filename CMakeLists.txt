cmake_minimum_required(VERSION 3.0)
project(wav2mp3)
include(ExternalProject)
enable_testing()

set(sources mutex.cpp thread.cpp wav_files.cpp)
set(headers lock_guard.h monitor.h mutex.h path.h thread.h wav_files.h)
set(tests)
add_executable(${PROJECT_NAME} ${sources} ${headers} main.cpp readme.txt)

add_executable(${PROJECT_NAME}-tests ${sources} ${headers} ${tests} main.test.cpp)

add_test(${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/samples)
add_test(${PROJECT_NAME}-tests ${PROJECT_NAME}-tests)
target_compile_options(${PROJECT_NAME}-tests PRIVATE $<$<CONFIG:Debug>:/MTd> $<$<NOT:$<CONFIG:Debug>>:/MT>)

# add support for <gtest/gtest.h>

ExternalProject_Add(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/googletest
  UPDATE_COMMAND ""
  INSTALL_COMMAND ""
)

add_library(googletest-static STATIC IMPORTED)
add_dependencies(googletest-static googletest)
target_link_libraries(${PROJECT_NAME}-tests googletest-static)

ExternalProject_Get_Property(googletest source_dir)
target_include_directories(${PROJECT_NAME}-tests PRIVATE "${source_dir}/googletest/include")

ExternalProject_Get_Property(googletest binary_dir)
set_property(TARGET googletest-static PROPERTY IMPORTED_LOCATION "${binary_dir}/googlemock/gtest/${CMAKE_CFG_INTDIR}/gtest.lib")

# add support for <lame.h>

ExternalProject_Add(lame
  URL http://downloads.sourceforge.net/project/lame/lame/3.99/lame-3.99.5.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/lame-3.99.5
  PATCH_COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/lame-CMakeLists.txt" "${CMAKE_CURRENT_BINARY_DIR}/lame-3.99.5/src/lame/CMakeLists.txt"
  COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/lame-3.99.5/src/lame/configMS.h" "${CMAKE_CURRENT_BINARY_DIR}/lame-3.99.5/src/lame/config.h"
  INSTALL_COMMAND ""
)

add_library(libmp3lame-static STATIC IMPORTED)
add_dependencies(libmp3lame-static lame)
target_link_libraries(${PROJECT_NAME} libmp3lame-static)
target_link_libraries(${PROJECT_NAME}-tests libmp3lame-static)

ExternalProject_Get_Property(lame source_dir)
target_include_directories(${PROJECT_NAME} PRIVATE "${source_dir}/include")
target_include_directories(${PROJECT_NAME}-tests PRIVATE "${source_dir}/include")

ExternalProject_Get_Property(lame binary_dir)
set_property(TARGET libmp3lame-static PROPERTY IMPORTED_LOCATION "${binary_dir}/${CMAKE_CFG_INTDIR}/libmp3lame-static.lib")

if (WIN32)

  # add support for <pthread.h>

  ExternalProject_Add(pthreads-win32
    URL http://downloads.sourceforge.net/project/pthreads4w/pthreads-w32-2-9-1-release.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/pthreads-win32-2.9.1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )

  ExternalProject_Get_Property(pthreads-win32 source_dir)
  target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "${source_dir}/Pre-built.2/include")
  target_include_directories(${PROJECT_NAME}-tests SYSTEM PRIVATE "${source_dir}/Pre-built.2/include")

  add_library(lib-pthreads-win32 STATIC IMPORTED)
  add_dependencies(lib-pthreads-win32 pthreads-win32)
  target_link_libraries(${PROJECT_NAME} lib-pthreads-win32)
  target_link_libraries(${PROJECT_NAME}-tests lib-pthreads-win32)

  if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set_property(TARGET lib-pthreads-win32 PROPERTY IMPORTED_LOCATION "${source_dir}/Pre-built.2/lib/x64/pthreadVC2.lib")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${source_dir}/Pre-built.2/dll/x64/pthreadVC2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
  else()
    set_property(TARGET lib-pthreads-win32 PROPERTY IMPORTED_LOCATION "${source_dir}/Pre-built.2/lib/x86/pthreadVC2.lib")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${source_dir}/Pre-built.2/dll/x86/pthreadVC2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
  endif()

endif ()

# improve solution clarity

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_target_properties(googletest lame pthreads-win32 PROPERTIES FOLDER "ExternalProjects")
set_target_properties(${PROJECT_NAME}-tests PROPERTIES FOLDER "Tests")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

source_group("Test Files" FILES ${tests})