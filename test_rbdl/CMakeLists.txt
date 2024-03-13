cmake_minimum_required(VERSION 3.10)

project(test_rbdl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

add_compile_options(-Wall)

find_package(spdlog REQUIRED)         # 个人常用的日志库
find_package(RBDL REQUIRED)           # 自动寻找RBDL的头文件和库文件
find_package(Eigen3 3.0.0 REQUIRED)   # RBDL依赖Eigen3

# 输出一些日志信息
message("RBDL_INCLUDE_DIR         : " ${RBDL_INCLUDE_DIR})
message("RBDL_LIBRARY             : " ${RBDL_LIBRARY})
message("RBDL_LUAMODEL_LIBRARY    : " ${RBDL_LUAMODEL_LIBRARY})
message("RBDL_URDFREADER_LIBRARY  : " ${RBDL_URDFREADER_LIBRARY})
message("EIGEN3_INCLUDE_DIR       : " ${EIGEN3_INCLUDE_DIR})

include_directories(${RBDL_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR})

add_subdirectory(utils)
add_subdirectory(1.first_program)
add_subdirectory(2.read_urdf)
add_subdirectory(3.pendulum)
add_subdirectory(4.constrained_double_pendulum)
add_subdirectory(5.test_kinematics)
add_subdirectory(6.learn_lua_model)
add_subdirectory(7.four_bar_linkage)

