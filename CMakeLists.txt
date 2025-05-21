cmake_minimum_required(VERSION 3.15...4.0)
project(
  cpp_raytracer
  VERSION 0.1
  LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE classes/*.cpp)
file(GLOB HEADERS classes/*.h)

add_executable(cpp_raytracer raytracer.cpp ${HEADERS} ${SOURCE})
 