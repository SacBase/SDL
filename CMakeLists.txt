CMAKE_MINIMUM_REQUIRED (VERSION 3.20)

PROJECT (sac-sdl)

# Where the compiled sac modules result
SET (DLL_BUILD_DIR "${PROJECT_BINARY_DIR}/lib")

# For what targets we build modules
# For what targets we build modules
SET (TARGETS    seq seq_checks mt_pth CACHE STRING "Build for these targets")
SET (SAC2C_EXEC                       CACHE STRING "Path to sac2c compiler")

# Check whether sac2c is operational
INCLUDE ("cmake-common/check-sac2c.cmake")
INCLUDE ("cmake-common/misc-macros.cmake")

# NB: Despite the name SDL2.sac, the module still relies on the "old" libSDL v1, not libSDL2.
SET (SDL_BUILDING_LIBRARY ON)
FIND_PACKAGE (SDL REQUIRED)

FIND_PACKAGE (X11 REQUIRED)

SET (SAC2C_EXTRA_INC "-I${SDL_INCLUDE_DIR}" "-I${X11_INCLUDE_DIR}"
     CACHE STRING "Extra include files that should be used by sac2c")
SET (SAC2C_CPP_INC "-Xl" "-lX11" "-Xtl" "-lX11")

# For every target run CMakeLists.txt in src
FOREACH (TARGET IN ITEMS ${TARGETS})
    ADD_SUBDIRECTORY (src src-${TARGET})
ENDFOREACH ()

# This build target is responsible for generating the package sac2crc file
CREATE_SAC2CRC_TARGET ("sdl" "${DLL_BUILD_DIR}" "${DLL_BUILD_DIR}" "")
