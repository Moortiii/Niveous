#include "Game.h"
#include "configuration/Configuration.h"
#include "components/Position.h"
#include "components/Velocity.h"
#include <GL/glew.h>
#include <fstream>
#include <iostream>

bool Game::initialize() {
    initializeSDL();
    initializeBox2D();
    initializeOpenGl();
    initializeSystems();
    initializeDelta();
    initializeEntities();

    running = true;

    return true;
}

// Main game loop
int Game::run() {
    while(running) {
        SDL_PollEvent(&event);

        // Allow the user to exit the game
        if(event.type == SDL_QUIT)
            running = false;

        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_r:
                    // Cover the screen in red
                    glClearColor(1.0, 0.0, 0.0, 1.0);
                    break;
                case SDLK_g:
                    // Cover the screen in green
                    glClearColor(0.0, 1.0, 0.0, 1.0);
                    break;
                case SDLK_b:
                    // Cover the screen in blue
                    glClearColor(0.0, 0.0, 1.0, 1.0);
                    break;
                default:
                    break;
            }
        }

        // Update all entities
        update();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Present the result
        SDL_GL_SwapWindow(window);
    }

    // Clean up and exit the game gracefully
    clean();

    return 0;
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::update() {
    updateDelta();

    for(auto& system : systems) {
        system->update(dt);
    }
}

void Game::updateDelta() {
    lastTick = currentTick;
    currentTick = SDL_GetPerformanceCounter();
    dt = ((currentTick - lastTick) * 1000 / (float)SDL_GetPerformanceFrequency());
}

bool Game::initializeOpenGl() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create OpenGL Rendering Context
    mainContext = SDL_GL_CreateContext(window);

    // Enable vertical sync
    SDL_GL_SetSwapInterval(1);

    // Init GLEW
    // Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
    #ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
    #endif

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);

    return true;
}

bool Game::initializeSystems() {
    systems.emplace_back(std::make_shared<MovementSystem>(registry));
    //systems.emplace_back(std::make_shared<RenderingSystem>(registry));

    return true;
}

bool Game::initializeBox2D() {
    b2Vec2 gravity(0.0f, -10.0f);
    world = std::make_shared<b2World>(gravity);

    // Used by the Box2D Iterative Solver
    timestep = 1.0f / 60.0f;
    velocityIterations = 6;
    positionIterations = 2;

    return true;
}

bool Game::initializeSDL() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to initialize SDL_VIDEO: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Niveous", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Configuration::SCREENWIDTH, Configuration::SCREENHEIGHT, SDL_WINDOW_OPENGL);

    if(!window) {
        SDL_Log("Unable to create SDL Window: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool Game::initializeDelta() {
    currentTick = SDL_GetPerformanceCounter();
    lastTick    = SDL_GetPerformanceCounter();
    dt = 0;

    return true;
}

bool Game::initializeEntities() {
    // Create an entity with a couple of components to test the systems
    auto entity = registry.create();

    registry.assign<Position>(entity, 0.0f, 0.0f);
    registry.assign<Velocity>(entity, 0.1f, 0.1f);

    return true;
}
