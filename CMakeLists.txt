cmake_minimum_required(VERSION 3.17)

set(PROJECT_NAME test_project)
set(TARGET_PROJECT "kbhit")
project(${PROJECT_NAME} LANGUAGES CXX)

# ------------------------------------------------------------------------------
# Set global variables
# ------------------------------------------------------------------------------
set(COMMON_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/common")
set(COMMON_LIBRARIES)

# ------------------------------------------------------------------------------
# Set Additional Find.cmake directory
# ------------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# ------------------------------------------------------------------------------
# Add subdirectory
# ------------------------------------------------------------------------------
add_subdirectory("src")
