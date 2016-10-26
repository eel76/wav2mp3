cmake_minimum_required(VERSION 3.1)
project(wav2mp3)

set(sources src/lame_encoder.cpp src/mp3.cpp src/mutex.cpp src/pcm.cpp src/thread.cpp src/wave_files.cpp src/wave_header.cpp lame/lame.cmake)
set(headers src/lame_encoder.h src/lame_encoder_exception.h src/lock_guard.h src/mp3.h src/mutex.h src/path.h src/pcm.h src/synchronize.h src/thread.h src/wave_files.h src/wave_format_exception.h src/wave_header.h)
set(tests src/mutex.test.cpp src/thread.test.cpp gtest/gtest.cmake)

include(gtest/gtest.cmake)
include(lame/lame.cmake)

if(WIN32)
  include(pthreads-win32/pthreads-win32.cmake)
  list(APPEND sources pthreads-win32/pthreads-win32.cmake)
  set(libraries lame pthreads-win32)
else()
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  set(libraries lame Threads::Threads stdc++fs)
endif()

add_executable(${PROJECT_NAME} ${sources} ${headers} src/main.cpp)
add_executable(${PROJECT_NAME}-unit-tests ${sources} ${headers} ${tests} src/main.test.cpp)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT_NAME}-unit-tests PROPERTY CXX_STANDARD 14)

target_link_libraries(${PROJECT_NAME} ${libraries})
target_link_libraries(${PROJECT_NAME}-unit-tests gtest ${libraries})

if(WIN32)
  get_property(pthreads-win32-location TARGET pthreads-win32 PROPERTY LOCATION)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${pthreads-win32-location}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
  add_custom_command(TARGET ${PROJECT_NAME}-unit-tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${pthreads-win32-location}" "$<TARGET_FILE_DIR:${PROJECT_NAME}-unit-tests>")
endif()

foreach(channels m s)
  foreach(samplerate 08 11 22 44)
    foreach(bitspersample 08 16)
	  set(file pcm${bitspersample}${samplerate}${channels}.wav)
      if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/samples/${file}")
        message(STATUS "Downloading sample file: ${file}")
        file(DOWNLOAD http://www.mauvecloud.net/sounds/${file} "${CMAKE_CURRENT_BINARY_DIR}/samples/${file}" SHOW_PROGRESS)
      endif()
    endforeach()
  endforeach()
endforeach()

enable_testing()
add_test(${PROJECT_NAME} ${PROJECT_NAME} "${CMAKE_CURRENT_BINARY_DIR}/samples")
add_test(${PROJECT_NAME}-unit-tests ${PROJECT_NAME}-unit-tests)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
source_group("External project Files" REGULAR_EXPRESSION "[.]cmake$")
