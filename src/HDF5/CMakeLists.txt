if(WIN32)
    # On Windows vcpkg systems, find_package(HDF5) should work
    find_package(hdf5 REQUIRED COMPONENTS CXX)
    set(_HDF5_LIBS hdf5::hdf5-shared hdf5::hdf5_hl-shared hdf5::hdf5_cpp-shared hdf5::hdf5_hl_cpp-shared)
else()
    # On Linux systems, we have to specify include dir and lib dir
    include(ProcessorCount)
    ProcessorCount(N)

    include(ExternalProject)
    set(ExternalProjectCMakeArgs
        -DHDF5_BUILD_CPP_LIB=ON
        )
    set(_hdf5_version "1.12.0")
    set(_hdf5_major_version "1.12")
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/hdf5_local/src)
    ExternalProject_Add(hdf5_local
        DOWNLOAD_COMMAND curl -s https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-${_hdf5_major_version}/hdf5-${_hdf5_version}/src/hdf5-${_hdf5_version}.tar.gz | tar xvz -C ${PROJECT_BINARY_DIR}/hdf5_local/src
        CMAKE_ARGS ${ExternalProjectCMakeArgs}
        SOURCE_DIR ${PROJECT_BINARY_DIR}/hdf5_local/src/hdf5-${_hdf5_version}
        BINARY_DIR ${PROJECT_BINARY_DIR}/hdf5_local/build
        STAMP_DIR  ${PROJECT_BINARY_DIR}/hdf5_local/stamp
        TMP_DIR    ${PROJECT_BINARY_DIR}/hdf5_local/tmp
        BUILD_COMMAND make -j${N}
        INSTALL_COMMAND ""
        )

    add_dependencies(${TARGET_PROJECT} hdf5_local)

    set(_HDF5_LIBS ${PROJECT_BINARY_DIR}/hdf5_local/build/bin/libhdf5_cpp.a ${PROJECT_BINARY_DIR}/hdf5_local/build/bin/libhdf5.a -ldl)

    target_include_directories(${TARGET_PROJECT} PRIVATE 
        ${PROJECT_BINARY_DIR}/hdf5_local/src/hdf5-${_hdf5_version}/src
        ${PROJECT_BINARY_DIR}/hdf5_local/src/hdf5-${_hdf5_version}/c++/src 
        ${PROJECT_BINARY_DIR}/hdf5_local/build
        )
endif()

file(GLOB_RECURSE CXX_HEADERS *.h *.hpp)
file(GLOB_RECURSE CXX_SOURCES *.cpp)
set(CXX_FILES ${CXX_HEADERS} ${CXX_SOURCES})
target_sources(${TARGET_PROJECT} PRIVATE ${CXX_FILES})

target_link_libraries(${TARGET_PROJECT} PRIVATE ${_HDF5_LIBS})