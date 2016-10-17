cmake_minimum_required (VERSION 3.0)
project(lame)

file(GLOB_RECURSE libmp3lame "libmp3lame/*.c")
file(GLOB_RECURSE mpglib "mpglib/*.c")
  
add_library(libmp3lame-static STATIC ${libmp3lame} ${mpglib})
target_include_directories(libmp3lame-static PUBLIC "include")
target_include_directories(libmp3lame-static PRIVATE "libmp3lame")
target_include_directories(libmp3lame-static PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(libmp3lame-static PRIVATE HAVE_CONFIG_H=1 _CRT_SECURE_NO_WARNINGS=1)
