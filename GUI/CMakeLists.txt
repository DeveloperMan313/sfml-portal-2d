cmake_minimum_required(VERSION 3.23)

set(PROJECT "game-gui")
project(${PROJECT})

set(${PROJECT}_SOURCES Menu.cpp Button.cpp)

set(${PROJECT}_HEADERS Menu.hpp Button.hpp)

set(${PROJECT}_SOURCE_LIST ${${PROJECT}_SOURCES} ${${PROJECT}_HEADERS})

add_library(${PROJECT} STATIC ${${PROJECT}_SOURCE_LIST})
