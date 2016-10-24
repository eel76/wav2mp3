cmake_minimum_required(VERSION 3.0)
project(wav2mp3)
enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

set(sources lame_encoder.cpp mp3.cpp mutex.cpp pcm.cpp thread.cpp wave_files.cpp wave_header.cpp lame/lame.cmake)
set(headers lame_encoder.h lame_encoder_exception.h lock_guard.h monitor.h mp3.h mutex.h path.h pcm.h thread.h wave_files.h wave_format_exception.h wave_header.h)
set(tests monitor.test.cpp mutex.test.cpp thread.test.cpp)

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
  file(DOWNLOAD https://www.starface.de/de/Service/moh/00_starface-music.wav "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}/00_starface-music.wav" SHOW_PROGRESS)
  file(DOWNLOAD https://www.starface.de/de/Service/moh/01_303-groove.wav "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}/01_303-groove.wav" SHOW_PROGRESS)
  file(DOWNLOAD https://www.starface.de/de/Service/moh/02_power-plant-soul.wav "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}/02_power-plant-soul.wav" SHOW_PROGRESS)
  file(DOWNLOAD https://www.starface.de/de/Service/moh/03_funky-70s.wav "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}/03_funky-70s.wav" SHOW_PROGRESS)
endif()

add_test(${PROJECT_NAME} ${PROJECT_NAME} "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}" >> "${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}/log.txt")
add_test(${PROJECT_NAME}-unit-tests ${PROJECT_NAME}-unit-tests)

source_group("External project Files" REGULAR_EXPRESSION "[.]cmake$")
