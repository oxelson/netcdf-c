set(libncpoco_SOURCES ncpoco.c ncpoco.h)

if(WIN32)
  set(libncpoco_SOURCES ${libncpoco_SOURCES} cp_win32.c)
else()
  set(libncpoco_SOURCES ${libncpoco_SOURCES} cp_unix.c)
endif()

add_library(ncpoco OBJECT ${libncpoco_SOURCES})

if (NETCDF_ENABLE_DLL)
  target_compile_definitions(ncpoco PRIVATE DLL_NETCDF DLL_EXPORT)
endif()

add_extra_dist(${libncpoco_SOURCES} CMakeLists.txt)
