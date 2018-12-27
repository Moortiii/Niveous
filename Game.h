#ifndef NIVEOUS_GAME_H
#define NIVEOUS_GAME_H

#include <SDL2/SDL.h>
#include <Box2D/Box2D.h>
#include "ECS/entt.hpp"
#include "systems/MovementSystem.h"

class Game {
public:
    bool initialize();
    int run();
    void clean();
    void update(float dt);
private:
    SDL_Window* window;
    SDL_Event event;
    float timestep;
    float dt;
    int32 velocityIterations;
    int32 positionIterations;
    std::shared_ptr<b2World> world;
    bool running;
    entt::registry<uint32_t> registry;

    std::shared_ptr<MovementSystem> movementSystem;

    std::vector<std::shared_ptr<ComponentSystem>> systems;
};


#endif //NIVEOUS_GAME_H
