cmake_minimum_required(VERSION 2.8)

find_package(Boost 1.56.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

option(TARGET_MIC "compile for Intel Xeon Phi")

set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -Wall -Werror -DNDEBUG -fopenmp")
if (TARGET_MIC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmic")
endif()

add_executable(axpy axpy.cpp)

