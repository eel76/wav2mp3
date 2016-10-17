cmake_minimum_required(VERSION 3.0)
project(wav2mp3)
include(ExternalProject)
enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

set(sources mutex.cpp thread.cpp wav_files.cpp)
set(headers lock_guard.h monitor.h mutex.h path.h thread.h wav_files.h)
set(tests)

add_executable(${PROJECT_NAME} ${sources} ${headers} main.cpp readme.txt)
add_executable(${PROJECT_NAME}-unit-tests ${sources} ${headers} ${tests} main.test.cpp)

add_test(${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/samples)
add_test(${PROJECT_NAME}-unit-tests ${PROJECT_NAME}-unit-tests)

include(googletest.cmake)
include(lame.cmake)

target_link_libraries(${PROJECT_NAME} libmp3lame-static)
target_link_libraries(${PROJECT_NAME}-unit-tests googletest-static libmp3lame-static)

if(WIN32)
  include(pthreads-win32.cmake)

  target_link_libraries(${PROJECT_NAME} lib-pthreads-win32)
  target_link_libraries(${PROJECT_NAME}-unit-tests lib-pthreads-win32)

  set_target_properties(pthreads-win32 PROPERTIES FOLDER "ExternalProjects")
endif()

set_target_properties(googletest lame PROPERTIES FOLDER "ExternalProjects")
