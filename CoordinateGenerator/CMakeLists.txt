file(GLOB headersCG ${headersCG} ${CMAKE_SOURCE_DIR}/CoordinateGenerator/Coordinates.h)
file(GLOB sourcesCG ${sourcesCG} ${CMAKE_SOURCE_DIR}/CoordinateGenerator/Coordinates.cpp)

set(TEST_EXECUTABLES ${TEST_EXECUTABLES}
${CMAKE_SOURCE_DIR}/CoordinateGenerator/TestCoordinates.cpp
${CMAKE_SOURCE_DIR}/CoordinateGenerator/Visualize.cpp
)

macro(build_executables EXECUTABLES)
foreach(EXECUTABLE ${EXECUTABLES})
  get_filename_component(TARGET_NAME ${EXECUTABLE} NAME_WE)
  add_executable(${TARGET_NAME} ${EXECUTABLE} ${sourcesCG} ${headersCG})
  target_link_libraries(${TARGET_NAME} ${ROOT_LIBRARIES} Geom)
endforeach()
endmacro()

build_executables("${TEST_EXECUTABLES}")
