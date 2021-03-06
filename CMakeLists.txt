cmake_minimum_required(VERSION 2.8.8)
project(Tracking)

#To put all the executables in build directory
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}) 

enable_language(CXX)

#Added to detect compiler support for C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

################################################################################
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()
string(TOUPPER ${CMAKE_BUILD_TYPE} _build_type)

# Setting the C++ compiler to use
set("${CMAKE_CXX_COMPILER_ID}" TRUE CACHE STRING "C++ Compiler")
if (NOT GNU AND NOT Clang AND NOT Intel)
  message(WARNING "Unsupported compiler. Build will likely fail.")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules)

include(MacroUtilities)
################################################################################

# see if we have CCACHE ( to avoid recompilation on branch switches )
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  message(STATUS "found ccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

option(VISUALIZATION "Enable or Disable Visualization" ON)
option(OPENGL "Use OpenGL for Visualization" OFF)
option(EVE "Use Eve for Visualization" ON)
option(UNITTEST "Enable various Unit tests" ON)
option(ROOT "Enable ROOT" ON)
option(EFF_AND "Efficiency will be calculated only if both plane fires" ON)
option(EFF_SETUP_AND "Efficiency w.r.t full Setup, i.e all the detector fires for an event" ON)
option(CLUSTER_SIZE "Allow to restrict the Cluster Size cut, default value will be 2 if switched ON." ON)
option(ACCUMULATE_TRACK "All the tracks will be displayed" OFF)
option(ANG_DIST "To calculate the angular distribution from tracks" OFF)



if(ANG_DIST)
if(ACCUMULATE_TRACK)
else()
message(STATUS "You have switched ON ANG_DIST but ACCUMULATE_TRACK is OFF, so i am switching ON ACCUMULATE_TRACK")
set(ACCUMULATE_TRACK ON)
endif(ACCUMULATE_TRACK)
add_definitions(-DANG_DIST)
endif(ANG_DIST)

if(ACCUMULATE_TRACK)
add_definitions(-DACCUMULATE_TRACK)
endif(ACCUMULATE_TRACK)

if(EFF_AND)
add_definitions(-DEFF_AND)
endif(EFF_AND)

if(EFF_SETUP_AND)
add_definitions(-DEFF_SETUP_AND)
endif(EFF_SETUP_AND)

if(CLUSTER_SIZE)
add_definitions(-DCLUSTER_SIZE)
endif(CLUSTER_SIZE)


if(VISUALIZATION)
add_definitions(-DSHOW_VISUALIZATION)
endif(VISUALIZATION)

if(VISUALIZATION AND EVE AND OPENGL)
message(STATUS "You have switched on both EVE and OPENGL, only one can work at a time, so i am Switching OFF both and using ROOTs default graphics")
set(EVE OFF)
set(OPENGL OFF)
endif()


if(VISUALIZATION AND EVE)
add_definitions(-DUSE_EVE)
else()
if(VISUALIZATION AND OPENGL)
add_definitions(-DUSE_OGL)
endif()
endif() 


if(ROOT)
add_definitions(-DUSE_ROOT)
find_package(ROOT REQUIRED)
include_directories(AFTER SYSTEM ${ROOT_INCLUDE_DIR})
set(EXTERNAL_LIBS ${EXTERNAL_LIBS}
${ROOT_LIBRARIES}
)

if(EVE)
set(EXTERNAL_LIBS ${EXTERNAL_LIBS}
Eve Geom
)
endif(EVE)

endif(ROOT)

#find_package(Geant4 REQUIRED)

#include_directories(${PROJECT_SOURCE_DIR})
#include_directories(${PROJECT_SOURCE_DIR}/inc)
#include_directories(${PROJECT_SOURCE_DIR}/visualizer)

#file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.h)
#file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
if(VISUALIZATION)
if(EVE)
file(GLOB headers ${headers} ${CMAKE_SOURCE_DIR}/visualizer/Eve/*.h)
file(GLOB sources ${sources}  ${CMAKE_SOURCE_DIR}/visualizer/Eve/*.cpp )
else()
file(GLOB headers ${headers} ${CMAKE_SOURCE_DIR}/visualizer/TGeo/Visualizer.h)
file(GLOB sources ${sources} ${CMAKE_SOURCE_DIR}/visualizer/TGeo/Visualizer.cpp )
endif(EVE)
endif(VISUALIZATION)

file(GLOB CFG_FILES ${PROJECT_SOURCE_DIR}/CfgFiles/*)
file(COPY ${CFG_FILES} DESTINATION ${PROJECT_SOURCE_DIR}/build)

#add_subdirectory(TreeExercise)
#add_subdirectory(test)
#add_subdirectory(TemplatizedInterface)
#add_subdirectory(CoordinateGenerator)

#COLLECTING ALL INCLUDE DIRECTORIES
set(INCLUDES ${INCLUDES}
${CMAKE_SOURCE_DIR}/CoordinateGenerator/inc
${CMAKE_SOURCE_DIR}/TemplatizedInterface/inc
#${PROJECT_SOURCE_DIR}/visualizer
${CMAKE_SOURCE_DIR}/inc
${CMAKE_SOURCE_DIR}
)



if(VISUALIZATION)
set(INCLUDES ${INCLUDES} ${CMAKE_SOURCE_DIR}/visualizer)
endif(VISUALIZATION)

macro(include_dirs INCLUDES)
  foreach(INCLUDE ${INCLUDES})
    include_directories(${INCLUDE})
  endforeach()
endmacro()

#COLLECTING ALL HEADER FILES
macro(collect_headers INCLUDES)
  foreach(INCLUDE ${INCLUDES})
    file(GLOB headers ${headers} ${INCLUDE}/*.h)
  endforeach()
endmacro()

#COLLECTING ALL SOURCE FILES
set(SOURCES ${SOURCES}
${CMAKE_SOURCE_DIR}/CoordinateGenerator/src
${CMAKE_SOURCE_DIR}/TemplatizedInterface/src
${CMAKE_SOURCE_DIR}/src
)

macro(collect_sources SOURCES)
  foreach(SOURCE ${SOURCES})
    file(GLOB sources ${sources} ${SOURCE}/*.cpp)
  endforeach()
endmacro()


SET(TESTSDIR ${TESTSDIR}
${CMAKE_SOURCE_DIR}/CoordinateGenerator/test
${CMAKE_SOURCE_DIR}/TemplatizedInterface/test
#${CMAKE_SOURCE_DIR}/test
)
#COLLECTING ALL TEST FILES
macro(collect_tests TESTS)
  foreach(TEST ${TESTS})
    file(GLOB tests ${tests} ${TEST}/*.cpp)
  endforeach()
endmacro()

macro(build_executables EXECUTABLES)
  foreach(EXECUTABLE ${EXECUTABLES})
    get_filename_component(TARGET_NAME ${EXECUTABLE} NAME_WE)
    add_executable(${TARGET_NAME} ${EXECUTABLE} ${sources} ${headers})
    target_link_libraries(${TARGET_NAME} ${EXTERNAL_LIBS})
#  if(EVE)
#    target_link_libraries(${TARGET_NAME} ${ROOT_LIBRARIES} Geom Eve)
#  else()
#    target_link_libraries(${TARGET_NAME} ${ROOT_LIBRARIES} Geom)
#  endif(EVE)
  endforeach()
endmacro()

message(STATUS ${headers})
include_dirs("${INCLUDES}")
collect_headers("${INCLUDES}")
collect_sources("${SOURCES}")
collect_tests("${TESTSDIR}")
build_executables("${tests}")

message(STATUS ${headers})
message(STATUS "====================================================")
message(STATUS ${tests})
message(STATUS "====================================================")
