cmake_minimum_required(VERSION 3.23)

set(PROJECT "sfml-portal-2d")

project(${PROJECT})

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_DIRS
    base/Logic
    base/Math
    base/Object
    base/Singleton
    base/UserInput
    events/Emitter
    events/Emitters
    graphics/Frame
    graphics/GUI
    graphics/Renderer
    graphics/Sprite
    graphics/Textures
    objects/Cube
    objects/Player
    objects/Portal
    objects/PortalProjectile
    objects/Wall
    physics/Hitbox
    physics/RBController
    physics/RigidBody
    physics/Simulation)

set(PROJECT_SOURCES
    base/Logic/Logic.cpp
    base/Math/Math.cpp
    base/Object/Object.cpp
    base/Singleton/Singleton.cpp
    base/UserInput/UserInput.cpp
    events/Emitter/Emitter.cpp
    events/Emitters/Emitters.cpp
    graphics/Frame/Frame.cpp
    graphics/GUI/Button.cpp
    graphics/GUI/Menu.cpp
    graphics/Renderer/Renderer.cpp
    graphics/Sprite/Sprite.cpp
    graphics/Textures/Textures.cpp
    objects/Cube/Cube.cpp
    objects/Player/Player.cpp
    objects/Portal/Portal.cpp
    objects/PortalProjectile/PortalProjectile.cpp
    objects/Wall/Wall.cpp
    physics/Hitbox/Hitbox.cpp
    physics/RBController/RBController.cpp
    physics/RigidBody/RigidBody.cpp
    physics/Simulation/Simulation.cpp)

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
