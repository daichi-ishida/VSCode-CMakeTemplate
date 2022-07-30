# find_package(XXX REQUIRED)

file(GLOB_RECURSE CXX_HEADERS *.h *.hpp)
file(GLOB_RECURSE CXX_SOURCES *.cpp)
set(CXX_FILES ${CXX_HEADERS} ${CXX_SOURCES})
target_sources(${TARGET} PRIVATE ${CXX_FILES})

# target_link_libraries(${TARGET} OpenGL::GL glfw)
# target_include_directories(${TARGET} PRIVATE ${COMMON_INCLUDE_DIRS})