add_executable(publisher ${CMAKE_CURRENT_SOURCE_DIR}/src/publisher.cpp)
lcm_target_link_libraries(publisher messages_cpp ${LCM_NAMESPACE}lcm)

add_executable(subscriber ${CMAKE_CURRENT_SOURCE_DIR}/src/subscriber.cpp)
lcm_target_link_libraries(subscriber messages_cpp ${LCM_NAMESPACE}lcm)