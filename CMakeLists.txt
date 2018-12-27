cmake_minimum_required(VERSION 3.12)

project(Niveous)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        Niveous
        main.cpp
        configuration/Configuration.h
        )

target_link_libraries(
        Niveous
        jsoncpp
        SDL2
        SDL2_image
        Box2D
        )