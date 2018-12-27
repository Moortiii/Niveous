#include "Game.h"
#include "configuration/Configuration.h"
#include "components/Position.h"
#include "components/Velocity.h"

bool Game::initialize() {

    // Initialize our Box2D World
    b2Vec2 gravity(0.0f, -10.0f);
    world = std::make_shared<b2World>(gravity);

    // Adjust values for the Box2D Iterative Solver
    timestep = 1.0f / 60.0f;
    velocityIterations = 6;
    positionIterations = 2;

    // Initialize the window we will render to
    window = SDL_CreateWindow("Niveous", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Configuration::SCREENWIDTH, Configuration::SCREENHEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

    if(!window) {
        SDL_Log("Unable to create SDL Window: %s", SDL_GetError());
        return false;
    }

    running = true;

    // Create systems for the ECS Architecture
    movementSystem = std::make_shared<MovementSystem>(registry);

    // Add our systems to the list to ensure it is later updated
    systems.push_back(movementSystem);

    // Create an entity with a couple of components to test the systems
    auto entity = registry.create();
    registry.assign<Position>(entity);
    registry.assign<Velocity>(entity, 0.1f, 0.1f);

    return true;
}

// Main game loop
int Game::run() {
    while(running) {
        SDL_PollEvent(&event);

        if(event.type == SDL_QUIT)
            running = false;

        // Update all entities
        update(0.01f);
    }

    // Clean up and exit the game gracefully
    clean();

    return 0;
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::update(float dt) {
    for(auto& system : systems) {
        system->update(dt);
    }
}
