CMAKE_MINIMUM_REQUIRED (VERSION 3.3)

PROJECT (sac-sdl)
#FIXME(artem) we can create a definitoin for language "SAC" which will
#             automatically pull sac2c compiler.

# Where the compiled sac modules result
SET (DLL_BUILD_DIR  "${PROJECT_BINARY_DIR}/lib")

# For what targets we build modules
SET (TARGETS            seq mt_pth  CACHE STRING "Build stdlib-jpeg for these targets")
SET (SAC2C_EXEC                     CACHE STRING "A path to sac2c compiler")
SET (LINKSETSIZE        "0"         CACHE STRING "Set a value for -linksetsize parameter of sac2c")
OPTION (BUILDGENERIC "Do not use architecture specific optimisations (useful for package builds)" OFF)

# Check whether sac2c is operational
INCLUDE ("cmake-common/check-sac2c.cmake")
INCLUDE ("cmake-common/check-sac2c-feature-support.cmake")
INCLUDE ("cmake-common/misc-macros.cmake")

# NB: Despite the name SDL2.sac, the module still relies on
# the "old" libSDL v1, not libSDL2.
FIND_PACKAGE (SDL REQUIRED)

SET (SAC2C_EXTRA_INC    "-I${SDL_INCLUDE_DIR}"
     CACHE STRING "Extra include files that should be used by sac2c")

SET (SAC2C_CPP_INC "")

# if building generically, we need to make sure sac2c supports this
IF (BUILDGENERIC)
    CHECK_SAC2C_SUPPORT_FLAG ("generic" "-generic")
    IF (HAVE_FLAG_generic)
        LIST (APPEND SAC2C_CPP_INC "-generic")
        MESSAGE (STATUS "Building with *no* system-specific optimisations.")
    ELSE ()
        MESSAGE (STATUS "Generic-build disabled as sac2c does not support this!")
        SET (BUILDGENERIC OFF)
    ENDIF ()
ENDIF ()

# TODO test if librt is ever needed, see SDL2:40, and add the check.

# For every target run CMakeLists.txt in src
FOREACH (TARGET IN ITEMS ${TARGETS})
    ADD_SUBDIRECTORY (src   src-${TARGET})
ENDFOREACH ()

# This build target is responsible for generating the package sac2crc file
CREATE_SAC2CRC_TARGET ("sdl" "${DLL_BUILD_DIR}" "${DLL_BUILD_DIR}" "")
