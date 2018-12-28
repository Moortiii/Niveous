#ifndef NIVEOUS_GAME_H
#define NIVEOUS_GAME_H

#include <SDL2/SDL.h>
#include <Box2D/Box2D.h>
#include "entt/entt.hpp"
#include "systems/MovementSystem.h"

class Game {
public:
    bool initialize();
    bool initializeOpenGl();
    bool initializeSystems();
    bool initializeBox2D();
    bool initializeSDL();
    bool initializeDelta();
    bool initializeEntities();
    void update();
    void updateDelta();
    int run();
    void clean();

private:
    // Game specific
    bool running;
    float dt;
    Uint64 currentTick;
    Uint64 lastTick;

    // SDL
    SDL_Window* window;
    SDL_Event event;
    SDL_GLContext mainContext;

    // Box2D
    float timestep;
    int32 velocityIterations;
    int32 positionIterations;
    std::shared_ptr<b2World> world;

    // Entt
    std::vector<std::shared_ptr<ComponentSystem>> systems;
    std::shared_ptr<MovementSystem> movementSystem;
    entt::registry<uint32_t> registry;
};


#endif //NIVEOUS_GAME_H
