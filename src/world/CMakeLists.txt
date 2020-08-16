# find_package(XXX REQUIRED)

file(GLOB_RECURSE CXX_HEADERS *.h *.hpp)
file(GLOB_RECURSE CXX_SOURCES *.cpp)
set(CXX_FILES ${CXX_HEADERS} ${CXX_SOURCES})
target_sources(${TARGET_PROJECT} PRIVATE ${CXX_FILES})

# target_link_libraries(${TARGET_PROJECT} OpenGL::GL glfw)
# target_include_directories(${TARGET_PROJECT} PRIVATE ${COMMON_INCLUDE_DIRS})