cmake_minimum_required(VERSION 3.23)

set(PROJECT "game-logic")
project(${PROJECT})

set(${PROJECT}_SOURCES Logic.cpp)

set(${PROJECT}_HEADERS Logic.hpp)

set(${PROJECT}_SOURCE_LIST ${${PROJECT}_SOURCES} ${${PROJECT}_HEADERS})

add_library(${PROJECT} STATIC ${${PROJECT}_SOURCE_LIST})

target_link_libraries(
  ${PROJECT}
  game-graphics
  game-player
  game-portal
  game-rigidbody
  game-simulation
  game-sprite
  game-textures
  game-wall
  game-emitter
  game-events)
