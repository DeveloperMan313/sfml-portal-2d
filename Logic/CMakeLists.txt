cmake_minimum_required(VERSION 3.23)

set(project "game-logic")
project(${project})

set(${project}_SOURCES Logic.cpp)

set(${project}_HEADERS Logic.hpp)

set(${project}_SOURCE_LIST ${${project}_SOURCES} ${${project}_HEADERS})

add_library(${project} STATIC ${${project}_SOURCE_LIST})
