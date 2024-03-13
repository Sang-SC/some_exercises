add_executable(2.read_urdf ${CMAKE_CURRENT_SOURCE_DIR}/src/2.read_urdf.cpp)
target_link_libraries(2.read_urdf ${RBDL_LIBRARY} ${RBDL_URDFREADER_LIBRARY} spdlog::spdlog)