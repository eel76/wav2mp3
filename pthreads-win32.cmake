ExternalProject_Add(pthreads-win32-build
  URL http://downloads.sourceforge.net/project/pthreads4w/pthreads-w32-2-9-1-release.zip
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/pthreads-win32-2.9.1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(pthreads-win32-build source_dir)
ExternalProject_Get_Property(pthreads-win32-build binary_dir)

add_library(pthreads-win32 SHARED IMPORTED)

file(MAKE_DIRECTORY "${source_dir}/Pre-built.2/include")
set_property(TARGET pthreads-win32 PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${source_dir}/Pre-built.2/include")

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set_property(TARGET pthreads-win32 PROPERTY IMPORTED_IMPLIB "${source_dir}/Pre-built.2/lib/x64/pthreadVC2.lib")
  set_property(TARGET pthreads-win32 PROPERTY IMPORTED_LOCATION "${source_dir}/Pre-built.2/dll/x64/pthreadVC2.dll")
else()
  set_property(TARGET pthreads-win32 PROPERTY IMPORTED_IMPLIB "${source_dir}/Pre-built.2/lib/x86/pthreadVC2.lib")
  set_property(TARGET pthreads-win32 PROPERTY IMPORTED_LOCATION "${source_dir}/Pre-built.2/dll/x86/pthreadVC2.dll")
endif()

add_dependencies(pthreads-win32 pthreads-win32-build)
