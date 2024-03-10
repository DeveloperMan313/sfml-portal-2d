cmake_minimum_required(VERSION 3.23)

set(project "game-graphics")
project(${project})

set(${project}_SOURCES graphics.cpp)

set(${project}_HEADERS graphics.hpp)

set(${project}_SOURCE_LIST ${${project}_SOURCES} ${${project}_HEADERS})

add_library(${project} STATIC ${${project}_SOURCE_LIST})
