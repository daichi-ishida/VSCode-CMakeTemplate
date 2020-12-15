# ------------------------------------------------------------------------------
# Configure Main Projects
# ------------------------------------------------------------------------------
add_executable(${TARGET_PROJECT})
set_target_properties(${TARGET_PROJECT} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

add_subdirectory(${TARGET_PROJECT})

target_include_directories(${TARGET_PROJECT} PRIVATE ${COMMON_INCLUDE_DIRS})

# ------------------------------------------------------------------------------
# Set compiler options
# ------------------------------------------------------------------------------

target_compile_features(${TARGET_PROJECT} PUBLIC cxx_std_14)
target_compile_options(${TARGET_PROJECT} PUBLIC
    # warnings
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic>
    $<$<CXX_COMPILER_ID:GCC>:-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>

    # flags for each build type
    $<$<CXX_COMPILER_ID:Clang>:
        $<$<CONFIG:Release>:-O3>                  # Release
        $<$<CONFIG:Debug>:-O0 -g>                 # Debug
        $<$<CONFIG:RelWithDebgInfo>:-O3 -g>       # RelWithDebInfo
    >
    $<$<CXX_COMPILER_ID:GCC>:
        $<$<CONFIG:Release>:-O3>                  # Release
        $<$<CONFIG:Debug>:-O0 -g>                 # Debug
        $<$<CONFIG:RelWithDebgInfo>:-O3 -g>       # RelWithDebInfo
    >

    $<$<CXX_COMPILER_ID:MSVC>: 
        $<$<CONFIG:Release>:/O2 /Zi /MP>          # Release
        $<$<CONFIG:Debug>:/Od /ZI /MP>            # Debug
        $<$<CONFIG:RelWithDebgInfo>:/O2 /ZI /MP>  # RelWithDebInfo
    >
)

target_compile_definitions(${TARGET_PROJECT} PUBLIC
    $<$<CONFIG:Debug>:DEBUG>          # Debug
    $<$<NOT:$<CONFIG:Debug>>:NDEBUG>  # else
)

# ------------------------------------------------------------------------------
# Post Process (copy shared files)
# ------------------------------------------------------------------------------

# These are run every time the main project is built:
add_custom_target(${PROJECT_NAME} ALL)

# Tell Visual Studio how to start the application:
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${TARGET_PROJECT}>
    VS_DEBUGGER_COMMAND $<TARGET_FILE:${TARGET_PROJECT}>)
