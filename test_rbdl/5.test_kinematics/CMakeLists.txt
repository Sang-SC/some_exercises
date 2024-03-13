add_executable(5.test_fk ${CMAKE_CURRENT_SOURCE_DIR}/src/5.test_fk.cpp)
target_link_libraries(5.test_fk ${RBDL_LIBRARY} ${RBDL_LUAMODEL_LIBRARY} csvtools spdlog::spdlog)

add_executable(5.test_ik ${CMAKE_CURRENT_SOURCE_DIR}/src/5.test_ik.cpp)
target_link_libraries(5.test_ik ${RBDL_LIBRARY} ${RBDL_LUAMODEL_LIBRARY} csvtools spdlog::spdlog)