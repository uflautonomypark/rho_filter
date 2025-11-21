cmake_minimum_required(VERSION 3.14)
project(rho_filter)

set(CMAKE_CXX_STANDARD 17)

# Try to find Eigen3 via Config mode (Homebrew standard)
find_package(Eigen3 3.3 QUIET CONFIG)

if(EIGEN3_FOUND)
    include_directories(${EIGEN3_INCLUDE_DIR})
else()
    # Fallback: Look in local directory if brew version isn't found
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/eigen)
endif()

add_executable(rhoFilter main.cpp rhoFilter.cpp)