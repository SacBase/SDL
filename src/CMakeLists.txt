INCLUDE ("${CMAKE_SOURCE_DIR}/cmake-common/sac2c-variables.cmake")
INCLUDE ("${CMAKE_SOURCE_DIR}/cmake-common/generate-sac2c-dependency-targets.cmake")

ADD_CUSTOM_TARGET (${TARGET}-all-modules ALL)

# C files
SET (C_DEPS_SRC
    src/sdl3.c
)

# SaC files
SET (SAC_SRC
    SDLdisplay.sac
)

# For every C source, compile an object file maintaining the right
# location in the binary directory so that SaC can pick it up
FOREACH (name ${C_DEPS_SRC})
    SET (src "${CMAKE_CURRENT_SOURCE_DIR}/${name}")

    GET_FILENAME_COMPONENT (dir ${name} DIRECTORY)
    GET_FILENAME_COMPONENT (dst ${name} NAME_WE)
    SET (dst "${CMAKE_CURRENT_BINARY_DIR}/${dir}/${dst}${OBJEXT}")

    # Put the object file in the same location as the source file
    FILE (MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${dir}")

    ADD_CUSTOM_COMMAND (
        OUTPUT "${dst}"
        MAIN_DEPENDENCY "${src}"
        IMPLICIT_DEPENDS C "${src}"
        COMMAND ${SAC2C} -v0 -noprelude -cc ccmod -o "${dst}" "${src}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${dir}"
        COMMENT "Generating ${dst} from ${src} for target ${TARGET}")
ENDFOREACH ()

# Make a directory for sac2c output
FILE (MAKE_DIRECTORY "${DLL_BUILD_DIR}/${TARGET_ENV}/${SBI}")

# For every SaC file, compile Tree and Mod files.
FOREACH (name ${SAC_SRC})
    SET (src "${CMAKE_CURRENT_SOURCE_DIR}/${name}")

    # sac2c requires object files relative to the working directory of the call to sac2c
    GET_FILENAME_COMPONENT (dir "${CMAKE_CURRENT_BINARY_DIR}/${name}" DIRECTORY)
    GET_FILENAME_COMPONENT (dst ${name} NAME_WE)
    SET (mod "${DLL_BUILD_DIR}/${TARGET_ENV}/${SBI}/lib${dst}Mod${VARIANT}${MODEXT}")
    SET (tree "${DLL_BUILD_DIR}/tree/${TARGET_ENV}/${SBI}/lib${dst}Tree${VARIANT}${TREE_DLLEXT}")

    RESOLVE_SAC_DEPS_AS_TARGETS ("${name}" "<TARGET>-module-<NAME>" target_list object_list source_list)
    LIST (APPEND deps_list ${target_list} ${object_list} ${source_list})
    MESSAGE (STATUS "Dependencies of ${name} are: ${deps_list}")
    UNSET (target_list)
    UNSET (object_list)
    UNSET (source_list)

    # Make sure that we have the directory we are changing to.
    FILE (MAKE_DIRECTORY "${dir}")

    ADD_CUSTOM_TARGET (${TARGET}-module-${dst} DEPENDS "${mod}" "${tree}")

    ADD_CUSTOM_COMMAND (
        OUTPUT ${mod} ${tree}
        COMMAND ${SAC2C} -v0 -Xc "\"$(pkg-config --cflags --libs sdl3)\"" -o ${DLL_BUILD_DIR} "${src}"
        WORKING_DIRECTORY "${dir}"
        MAIN_DEPENDENCY "${src}"
        DEPENDS ${deps_list}
        COMMENT "Building module ${dst} for target ${TARGET}")

    # Install compiled Tree/Mod parts to the corresponding locations
    INSTALL (
        FILES "${mod}"
        DESTINATION ${_install_mod_dir}/${TARGET_ENV}/${SBI}
        COMPONENT modules)
    INSTALL (
        FILES "${tree}"
        DESTINATION ${_install_tree_dir}/tree/${TARGET_ENV}/${SBI}
        COMPONENT trees)

    UNSET (deps_list)
ENDFOREACH ()
