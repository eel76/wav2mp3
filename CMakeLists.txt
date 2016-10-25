cmake_minimum_required(VERSION 3.1)
project(wav2mp3)
enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

set(sources src/lame_encoder.cpp src/mp3.cpp src/mutex.cpp src/pcm.cpp src/thread.cpp src/wave_files.cpp src/wave_header.cpp lame/lame.cmake)
set(headers src/lame_encoder.h src/lame_encoder_exception.h src/lock_guard.h src/monitor.h src/mp3.h src/mutex.h src/path.h src/pcm.h src/thread.h src/wave_files.h src/wave_format_exception.h src/wave_header.h)
set(tests src/monitor.test.cpp src/mutex.test.cpp src/thread.test.cpp)

include(gtest/gtest.cmake)
include(lame/lame.cmake)

if(WIN32)
  include(pthreads-win32/pthreads-win32.cmake)
  list(APPEND sources pthreads-win32/pthreads-win32.cmake)
  set(pthreads pthreads-win32)
else()
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  set(pthreads Threads::Threads)
endif()

add_executable(${PROJECT_NAME} ${sources} ${headers} src/main.cpp)
add_executable(${PROJECT_NAME}-unit-tests ${tests} ${sources} ${headers} src/main.test.cpp gtest/gtest.cmake)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT_NAME}-unit-tests PROPERTY CXX_STANDARD 14)

target_link_libraries(${PROJECT_NAME} lame ${pthreads})
target_link_libraries(${PROJECT_NAME}-unit-tests gtest lame ${pthreads})

if(WIN32)
  get_property(location TARGET pthreads-win32 PROPERTY LOCATION)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${location}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
  add_custom_command(TARGET ${PROJECT_NAME}-unit-tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${location}" "$<TARGET_FILE_DIR:${PROJECT_NAME}-unit-tests>")
else()
  target_link_libraries(${PROJECT_NAME} stdc++fs)
  target_link_libraries(${PROJECT_NAME}-unit-tests stdc++fs)
endif()

if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}")
  message(STATUS "Downloading sample files")
  file(DOWNLOAD http://www.mauvecloud.net/sounds/pcm1608m.wav "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}/pcm1608m.wav" SHOW_PROGRESS)
  file(DOWNLOAD http://www.mauvecloud.net/sounds/pcm1608s.wav "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}/pcm1608s.wav" SHOW_PROGRESS)
  file(DOWNLOAD http://www.mauvecloud.net/sounds/pcm1611m.wav "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}/pcm1611m.wav" SHOW_PROGRESS)
  file(DOWNLOAD http://www.mauvecloud.net/sounds/pcm1611s.wav "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}/pcm1611s.wav" SHOW_PROGRESS)
  file(DOWNLOAD http://www.mauvecloud.net/sounds/pcm1622m.wav "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}/pcm1622m.wav" SHOW_PROGRESS)
  file(DOWNLOAD http://www.mauvecloud.net/sounds/pcm1622s.wav "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}/pcm1622s.wav" SHOW_PROGRESS)
  file(DOWNLOAD http://www.mauvecloud.net/sounds/pcm1644m.wav "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}/pcm1644m.wav" SHOW_PROGRESS)
  file(DOWNLOAD http://www.mauvecloud.net/sounds/pcm1644s.wav "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}/pcm1644s.wav" SHOW_PROGRESS)
endif()

add_test(${PROJECT_NAME} ${PROJECT_NAME} "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}")
add_test(${PROJECT_NAME}-unit-tests ${PROJECT_NAME}-unit-tests)

source_group("External project Files" REGULAR_EXPRESSION "[.]cmake$")
