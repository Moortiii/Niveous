cmake_minimum_required(VERSION 3.12)

project(Niveous)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        Niveous
        configuration/Configuration.h
        ECS/entt.hpp
        Game.cpp
        Game.h
        main.cpp
        components/Position.h components/Velocity.h systems/MovementSystem.h systems/MovementSystem.cpp systems/ComponentSystem.cpp systems/ComponentSystem.h)

target_link_libraries(
        Niveous
        jsoncpp
        SDL2
        SDL2_image
        Box2D
        GLEW
        OpenGL
        )