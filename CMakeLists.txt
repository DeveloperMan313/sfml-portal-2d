cmake_minimum_required(VERSION 3.23)

set(project "sfml-portal-2d")
project(${project})

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(logic)
add_subdirectory(graphics)

add_executable(${project} main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(
  SFML 2.6
  COMPONENTS window graphics system
  REQUIRED)

target_link_libraries(${project} sfml-window sfml-graphics sfml-system
                      game-logic game-graphics)

if(NOT WIN32)
  target_link_libraries(${project} m)
endif()
