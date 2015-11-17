cmake_minimum_required(VERSION 2.8)

find_package(Boost 1.56.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

option(TARGET_MIC "compile for Intel Xeon Phi")

set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -Wall -Werror -DNDEBUG -fopenmp -opt-streaming-stores=auto")

if (TARGET_MIC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmic")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qopt-report=5 -qopt-report-phase=vec")
    
if (NOT TARGET_MIC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -xhost")
endif()

include_directories("${PROJECT_SOURCE_DIR}")
add_executable(philosophers main.cpp)

