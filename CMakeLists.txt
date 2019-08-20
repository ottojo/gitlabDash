cmake_minimum_required(VERSION 3.14)
project(dashServer)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(tinyxml2)

add_subdirectory(mstch)
include_directories(${mstch_INCLUDE_DIR})

add_subdirectory(cpp-httplib)

add_executable(dashServer main.cpp issue/Issue.cpp issue/Issue.h person/Person.cpp person/Person.h)
target_link_libraries(dashServer tinyxml2)
target_link_libraries(dashServer mstch)
target_link_libraries(dashServer httplib)
target_link_libraries(dashServer pthread)
target_link_libraries(dashServer ssl)
target_link_libraries(dashServer crypto)