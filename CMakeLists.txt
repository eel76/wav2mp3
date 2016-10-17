cmake_minimum_required(VERSION 3.0)
project(wav2mp3)
include(ExternalProject)
enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

set(sources mutex.cpp thread.cpp wav_files.cpp)
set(headers lock_guard.h monitor.h mutex.h path.h thread.h wav_files.h)
set(tests)
set(modules gtest.cmake lame.cmake)

if(WIN32)
  list(APPEND modules pthreads-win32.cmake)
  set(pthreads pthreads-win32)
endif()

foreach(module ${modules})
  include(${module})
endforeach()

add_executable(${PROJECT_NAME} ${modules} ${sources} ${headers} main.cpp readme.txt)
add_executable(${PROJECT_NAME}-unit-tests ${modules} ${sources} ${headers} ${tests} main.test.cpp)

add_test(${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/samples)
add_test(${PROJECT_NAME}-unit-tests ${PROJECT_NAME}-unit-tests)

target_link_libraries(${PROJECT_NAME} lame ${pthreads})
target_link_libraries(${PROJECT_NAME}-unit-tests gtest lame ${pthreads})

set_target_properties(gtest lame ${pthreads} PROPERTIES FOLDER "ExternalProjects")
source_group("Project Files" FILES ${modules})
