add_executable(7.four_bar_linkage1 ${CMAKE_CURRENT_SOURCE_DIR}/src/7.four_bar_linkage1.cpp)
target_link_libraries(7.four_bar_linkage1 ${RBDL_LIBRARY} ${RBDL_LUAMODEL_LIBRARY} csvtools spdlog::spdlog)

add_executable(7.four_bar_linkage2 ${CMAKE_CURRENT_SOURCE_DIR}/src/7.four_bar_linkage2.cpp)
target_link_libraries(7.four_bar_linkage2 ${RBDL_LIBRARY} ${RBDL_LUAMODEL_LIBRARY} csvtools spdlog::spdlog)

add_executable(7.test_ik_constraint ${CMAKE_CURRENT_SOURCE_DIR}/src/7.test_ik_constraint.cpp)
target_link_libraries(7.test_ik_constraint ${RBDL_LIBRARY} ${RBDL_LUAMODEL_LIBRARY} csvtools spdlog::spdlog)