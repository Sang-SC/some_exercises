add_executable(1.first_program ${CMAKE_CURRENT_SOURCE_DIR}/src/1.first_program.cpp)
target_link_libraries(1.first_program ${RBDL_LIBRARY} spdlog::spdlog)