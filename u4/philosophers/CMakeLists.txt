cmake_minimum_required(VERSION 2.8)

find_package(Boost 1.56.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

option(TARGET_MIC "compile for Intel Xeon Phi")
option(USE_GCC "use GCC options")
option(NO_WERROR "no warnings as errors")

set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -Wall -DNDEBUG -fopenmp")

if (NOT NO_WERROR)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

endif()

if (TARGET_MIC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmic")
endif()

if (USE_GCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftree-vectorize")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qopt-report=5 -qopt-report-phase=vec -opt-streaming-stores=auto")
    
    if (NOT TARGET_MIC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -xhost")
    endif()

endif()


include_directories("${PROJECT_SOURCE_DIR}")
add_executable(philosophers main.cpp)

