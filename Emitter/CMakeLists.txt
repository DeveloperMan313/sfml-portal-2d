cmake_minimum_required(VERSION 3.23)

set(PROJECT "game-emitter")
project(${PROJECT})

set(${PROJECT}_SOURCES Emitter.cpp)

set(${PROJECT}_HEADERS Emitter.hpp)

set(${PROJECT}_SOURCE_LIST ${${PROJECT}_SOURCES} ${${PROJECT}_HEADERS})

add_library(${PROJECT} STATIC ${${PROJECT}_SOURCE_LIST})
