ExternalProject_Add(pthreads-win32
  URL http://downloads.sourceforge.net/project/pthreads4w/pthreads-w32-2-9-1-release.zip
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/pthreads-win32-2.9.1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(pthreads-win32 source_dir)
ExternalProject_Get_Property(pthreads-win32 binary_dir)

add_library(lib-pthreads-win32 STATIC IMPORTED)
target_include_directories(lib-pthreads-win32 SYSTEM INTERFACE "${source_dir}/Pre-built.2/include")

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set_property(TARGET lib-pthreads-win32 PROPERTY IMPORTED_LOCATION "${source_dir}/Pre-built.2/lib/x64/pthreadVC2.lib")
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${source_dir}/Pre-built.2/dll/x64/pthreadVC2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
else()
  set_property(TARGET lib-pthreads-win32 PROPERTY IMPORTED_LOCATION "${source_dir}/Pre-built.2/lib/x86/pthreadVC2.lib")
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${source_dir}/Pre-built.2/dll/x86/pthreadVC2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()

add_dependencies(lib-pthreads-win32 pthreads-win32)
