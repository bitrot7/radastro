find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_RAD_ASTRO gnuradio-rad_astro)

FIND_PATH(
    GR_RAD_ASTRO_INCLUDE_DIRS
    NAMES gnuradio/rad_astro/api.h
    HINTS $ENV{RAD_ASTRO_DIR}/include
        ${PC_RAD_ASTRO_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_RAD_ASTRO_LIBRARIES
    NAMES gnuradio-rad_astro
    HINTS $ENV{RAD_ASTRO_DIR}/lib
        ${PC_RAD_ASTRO_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-rad_astroTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_RAD_ASTRO DEFAULT_MSG GR_RAD_ASTRO_LIBRARIES GR_RAD_ASTRO_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_RAD_ASTRO_LIBRARIES GR_RAD_ASTRO_INCLUDE_DIRS)
