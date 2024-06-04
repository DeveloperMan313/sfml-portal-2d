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
    Cube
    Emitter
    Events)

set(PROJECT_SOURCES
    Logic/Logic.cpp
    Graphics/Graphics.cpp
    Sprite/Sprite.cpp
    RigidBody/RigidBody.cpp
    Textures/Textures.cpp
    Simulation/Simulation.cpp
    Math/Math.cpp
    Hitbox/Hitbox.cpp
    GUI/Button.cpp
    GUI/Menu.cpp
    Player/Player.cpp
    Portal/Portal.cpp
    Wall/Wall.cpp
    Cube/Cube.cpp
    Emitter/Emitter.cpp
    Events/Events.cpp)

include_directories(${PROJECT_DIRS})

add_executable(${PROJECT} main.cpp ${PROJECT_SOURCES})

set(SFML_STATIC_LIBRARIES TRUE)

find_package(
  SFML 2.6
  COMPONENTS window graphics system
  REQUIRED)

target_link_libraries(${PROJECT} sfml-window sfml-graphics sfml-system)

if(NOT WIN32)
  target_link_libraries(${PROJECT} m)
endif()
