cmake_minimum_required(VERSION 2.8)

project(neural)

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)

add_definitions(-std=c++11)

add_executable(${PROJECT_NAME} main.cpp neural.cpp spam.cpp)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
