add_executable(4.constrained_double_pendulum ${CMAKE_CURRENT_SOURCE_DIR}/src/4.constrained_double_pendulum.cpp)
target_link_libraries(4.constrained_double_pendulum ${RBDL_LIBRARY} ${RBDL_LUAMODEL_LIBRARY} csvtools spdlog::spdlog)
