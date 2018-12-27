#include "Game.h"
#include "configuration/Configuration.h"
#include "components/Position.h"
#include "components/Velocity.h"
#include <GL/glew.h>

bool Game::initialize() {

    // Initialize our Box2D World
    b2Vec2 gravity(0.0f, -10.0f);
    world = std::make_shared<b2World>(gravity);

    // Adjust values for the Box2D Iterative Solver
    timestep = 1.0f / 60.0f;
    velocityIterations = 6;
    positionIterations = 2;

    // Initialize SDL_VIDEO
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to initialize SDL_VIDEO: %s", SDL_GetError());
        return false;
    }

    // Initialize the window we will render to
    window = SDL_CreateWindow("Niveous", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Configuration::SCREENWIDTH, Configuration::SCREENHEIGHT, SDL_WINDOW_OPENGL);

    if(!window) {
        SDL_Log("Unable to create SDL Window: %s", SDL_GetError());
        return false;
    }

    running = true;

    // Create OpenGL Rendering Context
    mainContext = SDL_GL_CreateContext(window);

    setOpenGlAttributes();

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

        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_r:
                    // Cover with red and update
                    glClearColor(1.0, 0.0, 0.0, 1.0);
                    break;
                case SDLK_g:
                    // Cover with green and update
                    glClearColor(0.0, 1.0, 0.0, 1.0);
                    break;
                case SDLK_b:
                    // Cover with blue and update
                    glClearColor(0.0, 0.0, 1.0, 1.0);
                    break;
                default:
                    break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);

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

bool Game::setOpenGlAttributes() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    return true;
}
