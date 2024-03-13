add_executable(3.pendulum ${CMAKE_CURRENT_SOURCE_DIR}/src/3.pendulum.cpp)
target_link_libraries(3.pendulum ${RBDL_LIBRARY} ${RBDL_LUAMODEL_LIBRARY} csvtools spdlog::spdlog)