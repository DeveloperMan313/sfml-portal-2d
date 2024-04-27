cmake_minimum_required(VERSION 3.23)

set(PROJECT "game-sprite")
project(${PROJECT})

set(${PROJECT}_SOURCES Sprite.cpp)

set(${PROJECT}_HEADERS Sprite.hpp)

set(${PROJECT}_SOURCE_LIST ${${PROJECT}_SOURCES} ${${PROJECT}_HEADERS})

add_library(${PROJECT} STATIC ${${PROJECT}_SOURCE_LIST})
