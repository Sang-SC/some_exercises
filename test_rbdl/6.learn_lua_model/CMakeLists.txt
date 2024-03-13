add_executable(6.read_lua ${CMAKE_CURRENT_SOURCE_DIR}/src/6.read_lua.cpp)
target_link_libraries(6.read_lua ${RBDL_LIBRARY} ${RBDL_LUAMODEL_LIBRARY} spdlog::spdlog)