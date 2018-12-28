#include <SDL2/SDL_log.h>
#include "MovementSystem.h"
#include "../components/Position.h"

void MovementSystem::update(float dt) {
    auto view = registry.view<Position, Velocity>();

    view.each([dt](auto entity, auto& position, auto& velocity) {
        position.x += velocity.x * dt;
        position.y += velocity.y * dt;

        //SDL_Log("Position: %f, %f", position.x, position.y);
    });
}
