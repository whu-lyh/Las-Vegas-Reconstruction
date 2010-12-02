cmake_minimum_required(VERSION 2.8)
project(LSSR)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

link_directories("$ENV{HOME}/local/lib")
link_directories("${CMAKE_SOURCE_DIR}/lib")

include_directories(${LSSR_SOURCE_DIR}/ext/ann)
include_directories(${LSSR_SOURCE_DIR}/ext/newmat)
include_directories(${LSSR_SOURCE_DIR}/ext/stann)

set(ADDITIONAL_COMPILER_FLAGS "-O3 -fpermissive -Wno-deprecated -Wno-write-strings")

find_package( Boost COMPONENTS program_options filesystem  REQUIRED)
if(Boost_FOUND)
  link_directories(${BOOST_LIBRARY_DIRS})
  include_directories(${Boost_INCLUDE_DIRS})
  add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
endif()

find_package(OpenMP)
if(OpenMP_FOUND)
  link_directories(${OpenMP_LIBRARY_DIRS})
  include_directories(${OpenMP_INCLUDE_DIRS})
endif(OpenMP_FOUND)

find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
  link_directories(${OpenGL_INCLUDE_DIRS})
  include_directories(${OpenGL_LIBRARY_DIRS})
endif(OpenGL_FOUND)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_COMPILER_FLAGS}")

add_subdirectory(ext/newmat)
add_subdirectory(ext/ann)
add_subdirectory(src/mcubes)





