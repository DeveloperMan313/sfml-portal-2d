cmake_minimum_required(VERSION 3.23)

set(PROJECT "game-graphics")
project(${PROJECT})

set(${PROJECT}_SOURCES Graphics.cpp)

set(${PROJECT}_HEADERS Graphics.hpp)

set(${PROJECT}_SOURCE_LIST ${${PROJECT}_SOURCES} ${${PROJECT}_HEADERS})

add_library(${PROJECT} STATIC ${${PROJECT}_SOURCE_LIST})

target_link_libraries(${PROJECT} game-sprite)
