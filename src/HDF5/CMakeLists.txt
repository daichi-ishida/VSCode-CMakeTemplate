find_package(hdf5 CONFIG REQUIRED)

file(GLOB_RECURSE CXX_HEADERS *.h *.hpp)
file(GLOB_RECURSE CXX_SOURCES *.cpp)
set(CXX_FILES ${CXX_HEADERS} ${CXX_SOURCES})
target_sources(${TARGET_PROJECT} PRIVATE ${CXX_FILES})

target_link_libraries(${TARGET_PROJECT} PRIVATE hdf5::hdf5-shared hdf5::hdf5_hl-shared hdf5::hdf5_cpp-shared hdf5::hdf5_hl_cpp-shared)
# target_include_directories(${TARGET_PROJECT} PRIVATE ${COMMON_INCLUDE_DIRS})