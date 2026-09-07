CMAKE_MINIMUM_REQUIRED (VERSION 3.20)

PROJECT (sac-sdl)

# Where the compiled sac modules result
SET (DLL_BUILD_DIR "${PROJECT_BINARY_DIR}/lib")

# For what targets we build modules
SET (TARGETS    seq seq_checks mt_pth CACHE STRING "Build for these targets")
SET (SAC2C_EXEC                       CACHE STRING "Path to sac2c compiler")
OPTION (BUILDGENERIC "Do not use architecture-specific optimisations" OFF)

# Check whether sac2c is operational
LIST (APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-common")
INCLUDE ("cmake-common/check-sac2c.cmake")
INCLUDE ("cmake-common/check-sac2c-feature-support.cmake")
INCLUDE ("cmake-common/misc-macros.cmake")

SET (SDL_BUILDING_LIBRARY ON)
FIND_PACKAGE (SDL3 REQUIRED)

IF (SDL3_FOUND)
    MESSAGE (STATUS "SDL3 found")
    get_target_property (SDL3_INCLUDE_DIRS SDL3::Headers INTERFACE_INCLUDE_DIRECTORIES)
    SET (SAC2C_EXTRA_INC "-I${SDL3_INCLUDE_DIRS}")
ENDIF ()

# X11 is needed for X-forwarding over SSH
FIND_PACKAGE (X11 QUIET)

IF (X11_FOUND)
    MESSAGE (STATUS "X11 found")
    SET (SAC2C_CC_FLAGS "-DHAVE_X11")
    SET (SAC2C_EXTRA_INC "${SAC2C_EXTRA_INC}" "-I${X11_INCLUDE_DIR}")
    get_filename_component (X11_LIBRARY_PATH ${X11_X11_LIB} DIRECTORY)
    SET (SAC2C_CPP_INC "-Xl" "-L${X11_LIBRARY_PATH}" "-Xl" "-lX11" "-Xtl" "-L${X11_LIBRARY_PATH}" "-Xtl" "-lX11")
ENDIF ()

# Check if sac2c suppports building generically
IF (BUILDGENERIC)
    CHECK_SAC2C_SUPPORT_FLAG ("generic" "-generic")
    IF (HAVE_FLAG_generic)
        LIST (APPEND SAC2C_CPP_INC "-generic")
        MESSAGE (STATUS "Building with *no* system-specific optimisations")
    ELSE ()
        MESSAGE (STATUS "Generic-build disabled as sac2c does not support this")
        SET (BUILDGENERIC OFF)
    ENDIF ()
ENDIF ()

# For every target run CMakeLists.txt in src
FOREACH (TARGET IN ITEMS ${TARGETS})
    ADD_SUBDIRECTORY (src src-${TARGET})
ENDFOREACH ()

# This build target is responsible for generating the package sac2crc file
CREATE_SAC2CRC_TARGET ("sdl" "${DLL_BUILD_DIR}" "${DLL_BUILD_DIR}" "")
