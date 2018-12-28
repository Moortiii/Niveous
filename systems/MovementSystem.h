#ifndef NIVEOUS_MOVEMENTSYSTEM_H
#define NIVEOUS_MOVEMENTSYSTEM_H

#include "../entt/entity/registry.hpp"
#include "../components/Position.h"
#include "../components/Velocity.h"
#include "ComponentSystem.h"

class MovementSystem : public ComponentSystem {
public:
    explicit MovementSystem(entt::registry<uint32_t>& registry) : ComponentSystem(registry) {}
    void update(float dt) override;
};

#endif //NIVEOUS_MOVEMENTSYSTEM_H
