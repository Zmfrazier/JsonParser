project(json CXX)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(json ../src/json.cpp ../src/json.cpp)