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

#if("$ENV{CC}" STREQUAL "icc")
#    message(STATUS "Adding vectorization for icc")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qopt-report=5 -qopt-report-phase=vec")
#    
#    if (NOT TARGET_MIC)
#         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -xhost")
#    endif()
#    
#else()
#    message(STATUS "Adding vectorization for gcc")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftree-vectorize -ftree-vectorizer-verbose=2 -fopt-info-vec-missed")
#endif()

include_directories("${PROJECT_SOURCE_DIR}")
add_executable(matmat main.cpp matmat.cpp)

