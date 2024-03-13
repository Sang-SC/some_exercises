# 将csvtools.cc编译成库文件
add_library(csvtools SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/csvtools.cc)
target_include_directories(csvtools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)