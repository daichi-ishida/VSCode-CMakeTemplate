macro(_find_lib_from_path variable path)
if(NOT ${variable})
    file(GLOB _FOUND_FILES ${path})
    if(NOT _FOUND_FILES)
        message(WARNING "_find_lib_from_path_shared_library: Unable to find any files corresponding to the glob specification ${path}!")
    else()
        list(GET _FOUND_FILES 0 ${variable}) # Set variable to the first element of the list
    endif()
endif()
endmacro()

set(PROJ_OPENVDB_LIBRARIES_TO_COPY "")

if(WIN32)
    # Windows version - we assume you use vcpkg here or have set the
    if(NOT CMAKE_TOOLCHAIN_FILE)
        message(WARNING "PROJ_BUILD_OPENVDB was specified, but CMAKE_TOOLCHAIN_FILE wasn't specified. This is only an issue if you're using something like vcpkg. If you're using the CMake GUI, you'll want to specify the vcpkg toolchain in the configuration settings (delete the cache, press the Configure button, then choose 'Specify toolchain file for cross-compiling' and enter the path to vcpkg.cmake, then make sure to re-enable PROJ_BUILD_OPENVDB). If you're using the CMake command-line interface, you might need to specify -DCMAKE_TOOLCHAIN_FILE=<path to toolchain file>.")
    endif()
    
    # Linking OpenVDB with vcpkg is a bit odd, so this section is a bit more complex than the rest
    # of the CMakeFile. If you're not building with OpenVDB, this never gets included.
    #
    # As of this writing, vcpkg seems to assume you're running its CMake toolchain with a single 
    # configuration (e.g. Debug or Release), so find_package calls search first in the respective
    # debug or release directory in vcpkg's installed directory. Vcpkg also runs a useful PowerShell
    # script in the post-build process step that searches for referenced DLLs and automatically copies
    # them to the output directory. Here, it looks at your built executable's path, determines whether
    # you built in debug or release mode from that, and then searches in the relevant vcpkg directories.
    #
    # Unfortunately, it doesn't search in the release directory if it can't find the relevant file in
    # the debug directory, and silently fails if this is the case. At the same time, openvdb in vcpkg
    # has some quirks - for example, the debug version always uses the release version of Half and tbb.
    #
    # Here's how we handle this:
    # - Finding openvdb.h works normally.
    # - We find paths to separate debug and release libraries using the fact that (as of this writing!)
    # tbb's debug and release libraries and DLLs are named different things (tbb_debug and tbb). This
    # allows us to find vcpkg's install directories; these can also be overriden by the user.
    # - Finally, we make sure to always copy over the release versions of Half and tbb. When we're building
    # in debug mode, this allows us to include the DLLs vcpkg won't find, while this saves some work for
    # vcpkg in release mode.
    
    # Compile definitions: We're using the DLL of OpenVDB, instead of building it statically.
    # TBB has a #pragma comment(lib, tbb...) line in it, but this isn't set up to work properly,
    # so we explicitly link it instead:
    target_compile_definitions(${TARGET_PROJECT} PUBLIC __TBB_NO_IMPLICIT_LINKAGE)
    
    # Find the path to the include directory:
    if(NOT PROJ_OPENVDB_INCLUDE_DIR)
        find_path(PROJ_OPENVDB_INCLUDE_DIR openvdb/openvdb.h)
    endif()
    message(STATUS "Using this path for PROJ_OPENVDB_INCLUDE_DIR: ${PROJ_OPENVDB_INCLUDE_DIR}")
    target_include_directories(${TARGET_PROJECT} PUBLIC ${PROJ_OPENVDB_INCLUDE_DIR})
    
    # Find the debug and release directories containing vcpkg's .lib files:
    if(NOT PROJ_OPENVDB_LIB_RELEASE_DIR)
        find_path(PROJ_OPENVDB_LIB_RELEASE_DIR tbb.lib PATH_SUFFIXES lib)
    endif()
    if(NOT PROJ_OPENVDB_LIB_DEBUG_DIR)
        find_path(PROJ_OPENVDB_LIB_DEBUG_DIR tbb_debug.lib PATH_SUFFIXES debug/lib)
    endif()
    
    # Find the paths to our .lib files manually:
    _find_lib_from_path(PROJ_OPENVDB_LIB_DEBUG ${PROJ_OPENVDB_LIB_DEBUG_DIR}/openvdb_d.lib)
    # Compatibility for OpenVDB < 7.0
    _find_lib_from_path(PROJ_OPENVDB_LIB_DEBUG ${PROJ_OPENVDB_LIB_DEBUG_DIR}/openvdb.lib)
    _find_lib_from_path(PROJ_TBB_LIB_DEBUG     ${PROJ_OPENVDB_LIB_DEBUG_DIR}/tbb_debug.lib)
    _find_lib_from_path(PROJ_BLOSC_LIB_DEBUG   ${PROJ_OPENVDB_LIB_DEBUG_DIR}/blosc.lib)
    _find_lib_from_path(PROJ_ZLIB_LIB_DEBUG    ${PROJ_OPENVDB_LIB_DEBUG_DIR}/zlib*.lib)
    _find_lib_from_path(PROJ_HALF_LIB_DEBUG    ${PROJ_OPENVDB_LIB_DEBUG_DIR}/Half-*.lib)
    
    _find_lib_from_path(PROJ_OPENVDB_LIB_RELEASE ${PROJ_OPENVDB_LIB_RELEASE_DIR}/openvdb.lib)
    _find_lib_from_path(PROJ_TBB_LIB_RELEASE     ${PROJ_OPENVDB_LIB_RELEASE_DIR}/tbb.lib)
    _find_lib_from_path(PROJ_BLOSC_LIB_RELEASE   ${PROJ_OPENVDB_LIB_RELEASE_DIR}/blosc.lib)
    _find_lib_from_path(PROJ_ZLIB_LIB_RELEASE    ${PROJ_OPENVDB_LIB_RELEASE_DIR}/zlib*.lib)
    _find_lib_from_path(PROJ_HALF_LIB_RELEASE    ${PROJ_OPENVDB_LIB_RELEASE_DIR}/Half-*.lib)
    
    target_link_libraries(${TARGET_PROJECT} PRIVATE
        debug ${PROJ_OPENVDB_LIB_DEBUG}
        debug ${PROJ_TBB_LIB_DEBUG}
        debug ${PROJ_BLOSC_LIB_DEBUG}
        debug ${PROJ_ZLIB_LIB_DEBUG}
        debug ${PROJ_HALF_LIB_DEBUG}
        optimized ${PROJ_OPENVDB_LIB_RELEASE}
        optimized ${PROJ_TBB_LIB_RELEASE}
        optimized ${PROJ_BLOSC_LIB_RELEASE}
        optimized ${PROJ_ZLIB_LIB_RELEASE}
        optimized ${PROJ_HALF_LIB_RELEASE}
    )
    
    # Find the path to the directory containing release versions of DLLs.
    if(NOT PROJ_OPENVDB_DLL_RELEASE_DIR)
        find_path(PROJ_OPENVDB_DLL_RELEASE_DIR tbb.dll PATH_SUFFIXES bin)
    endif()
    
    # Include the release versions of Half and tbb.
    _find_lib_from_path(PROJ_TBB_DLL_RELEASE ${PROJ_OPENVDB_DLL_RELEASE_DIR}/tbb.dll)
    _find_lib_from_path(PROJ_HALF_DLL_RELEASE ${PROJ_OPENVDB_DLL_RELEASE_DIR}/Half-*.dll)
    list(APPEND PROJ_OPENVDB_LIBRARIES_TO_COPY ${PROJ_TBB_DLL_RELEASE} ${PROJ_HALF_DLL_RELEASE})
else()
    # On Linux systems, find_package(openvdb) should work
    find_package(OpenVDB REQUIRED)
    target_link_libraries(${TARGET_PROJECT} PUBLIC OpenVDB::openvdb)
endif()

file(GLOB_RECURSE CXX_HEADERS *.h *.hpp)
file(GLOB_RECURSE CXX_SOURCES *.cpp)
set(CXX_FILES ${CXX_HEADERS} ${CXX_SOURCES})
target_sources(${TARGET_PROJECT} PRIVATE ${CXX_FILES})

if(PROJ_OPENVDB_LIBRARIES_TO_COPY)
    # Copy OpenVDB and the libraries it depends upon
    add_custom_command(OUTPUT copiedSharedLibraries.stamp
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PROJ_OPENVDB_LIBRARIES_TO_COPY} $<TARGET_FILE_DIR:${TARGET_PROJECT}>
    COMMAND_EXPAND_LISTS)
    add_custom_target(OPENVDB_LIB_COPY SOURCES copiedSharedLibraries.stamp)
endif()
