cmake_minimum_required(VERSION 3.14)
project(rho_filter)

set(CMAKE_CXX_STANDARD 17)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(include ${Eigen3_INCLUDE_DIRS})

# Compile the core logic as a separate library
add_library(rho_filter_lib src/rhoFilter.cpp)
target_link_libraries(rho_filter_lib Eigen3::Eigen)

# Compile the ROS node
add_executable(rho_filter_node src/rho_node.cpp)
ament_target_dependencies(rho_filter_node rclcpp std_msgs)

# Link the node to the logic library and Eigen
target_link_libraries(rho_filter_node rho_filter_lib Eigen3::Eigen)

install(TARGETS
  rho_filter_lib
  rho_filter_node
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

ament_package()