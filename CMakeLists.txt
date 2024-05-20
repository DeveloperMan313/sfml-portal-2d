cmake_minimum_required(VERSION 3.23)

set(PROJECT "sfml-portal-2d")
project(${PROJECT})

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_DIRS
    Logic
    Graphics
    Sprite
    RigidBody
    Textures
    Simulation
    Math
    Hitbox
    GUI
    Player
    Portal
    Wall
    Cube)

foreach(PROJECT_DIR IN LISTS PROJECT_DIRS)
  add_subdirectory(${PROJECT_DIR})
endforeach()

include_directories(${PROJECT_DIRS})

add_executable(${PROJECT} main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(
  SFML 2.6
  COMPONENTS window graphics system
  REQUIRED)

set(GAME_COMPONENTS
    game-logic
    game-graphics
    game-sprite
    game-rigidbody
    game-textures
    game-simulation
    game-math
    game-hitbox
    game-gui
    game-player
    game-portal
    game-wall
    game-cube)

set(SFML_COMPONENTS sfml-window sfml-graphics sfml-system)

foreach(PROJECT_COMPONENT IN LISTS GAME_COMPONENTS COMPONENTS PROJECT)
  target_link_libraries(${PROJECT_COMPONENT} ${SFML_COMPONENTS})
  if(NOT WIN32)
    target_link_libraries(${PROJECT_COMPONENT} m)
  endif()
endforeach()

target_link_libraries(${PROJECT} ${GAME_COMPONENTS})
