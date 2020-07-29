function(BUILD_PROJECT PROJNAME)
  set(TARGET_PROJECT_DIR "${CMAKE_CURRENT_LIST_DIR}/${PROJNAME}")

  if (EXISTS "${TARGET_PROJECT_DIR}/common.h.in")
    configure_file("${TARGET_PROJECT_DIR}/common.h.in"
                   "${TARGET_PROJECT_DIR}/common.h" @ONLY)
  endif()

  # Source files
  file(GLOB SOURCE_FILES "${PROJNAME}/*.cpp" "${PROJNAME}/*.h")
  file(GLOB SHADER_FILES "${PROJNAME}/shaders/*.vert" "${PROJNAME}/shaders/*.frag")

  include_directories(${ALL_INCLUDE_DIRS})
  add_executable(${PROJNAME} ${SOURCE_FILES} ${SHADER_FILES})
  target_link_libraries(${PROJNAME} ${ALL_LIBRARIES})

  set_target_properties(${PROJNAME} PROPERTIES FOLDER "Contents")

  source_group("Source Files" FILES ${SOURCE_FILES})
  source_group("Shader Files" FILES ${SHADER_FILES})
endfunction(BUILD_PROJECT)

file(GLOB SUBDIR_LIST RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*")
foreach(SUBDIR ${SUBDIR_LIST})
  if (IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${SUBDIR}")
    BUILD_PROJECT(${SUBDIR})
  endif()
endforeach()
