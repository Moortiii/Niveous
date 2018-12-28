cmake_minimum_required(VERSION 3.12)

project(Niveous)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        Niveous
        configuration/Configuration.h
        components/Position.h
        components/Velocity.h
        entt/entt.hpp
        Game.cpp
        Game.h
        main.cpp
        systems/ComponentSystem.cpp
        systems/ComponentSystem.h
        systems/MovementSystem.h
        systems/MovementSystem.cpp
        utilities/ResourceManager.cpp
        utilities/ResourceManager.h
        utilities/Shader.cpp
        utilities/Shader.h
        utilities/Texture.cpp
        utilities/Texture.h
        )

target_link_libraries(
        Niveous
        Box2D
        GLEW
        jsoncpp
        OpenGL
        SDL2
        SDL2_image
        SOIL
        )