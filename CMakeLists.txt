cmake_minimum_required(VERSION 3.0)
project(wav2mp3)
enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

set(sources mutex.cpp thread.cpp wav_files.cpp lame/lame.cmake)
set(headers lock_guard.h monitor.h mutex.h path.h thread.h wav_files.h)
set(tests monitor.test.cpp thread.test.cpp)

include(gtest/gtest.cmake)
include(lame/lame.cmake)

if(WIN32)
  include(pthreads-win32/pthreads-win32.cmake)
  list(APPEND sources pthreads-win32/pthreads-win32.cmake)
  set(pthreads pthreads-win32)
endif()

add_executable(${PROJECT_NAME} ${sources} ${headers} main.cpp readme.txt)
add_executable(${PROJECT_NAME}-unit-tests ${tests} ${sources} ${headers} main.test.cpp gtest/gtest.cmake)

target_link_libraries(${PROJECT_NAME} lame ${pthreads})
target_link_libraries(${PROJECT_NAME}-unit-tests gtest lame ${pthreads})

if(WIN32)
  get_property(location TARGET pthreads-win32 PROPERTY LOCATION)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${location}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
  add_custom_command(TARGET ${PROJECT_NAME}-unit-tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${location}" "$<TARGET_FILE_DIR:${PROJECT_NAME}-unit-tests>")
endif()

add_test(${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/samples)
add_test(${PROJECT_NAME}-unit-tests ${PROJECT_NAME}-unit-tests)

source_group("External project Files" REGULAR_EXPRESSION "[.]cmake$")
