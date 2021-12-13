cmake_minimum_required(VERSION 3.0.0)
project(ToyRobotCodeChallenge VERSION 0.1.0)

file( GLOB SOURCES "*.cpp")

add_executable(ToyRobotCodeChallenge ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack) 
