cmake_minimum_required(VERSION 3.0.2)
project(line_follower)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
)
find_package(OpenCV REQUIRED) # 找OpenCV 4.2 安装路径

catkin_package()
include_directories(
include
  ${catkin_INCLUDE_DIRS}
)

add_library(webotsInterface 
  include/line_follower/webotsInterface.hpp
  src/webotsInterface.cpp
)

add_library(level1
  include/line_follower/level1.hpp
  src/level1.cpp
)

add_library(level2
  include/line_follower/level2.hpp
  src/level2.cpp
)

add_executable(test_opencv src/test_opencv.cpp)
target_link_libraries(test_opencv	${catkin_LIBRARIES})
target_link_libraries(test_opencv	${OpenCV_LIBRARIES})
target_link_libraries(webotsInterface	${OpenCV_LIBRARIES})

add_executable(main src/main.cpp)
add_dependencies(main ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(main webotsInterface level1 level2 ${catkin_LIBRARIES})