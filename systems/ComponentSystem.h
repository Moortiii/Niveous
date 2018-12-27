#ifndef NIVEOUS_COMPONENTSYSTEM_H
#define NIVEOUS_COMPONENTSYSTEM_H

#include "../ECS/entt.hpp"

class ComponentSystem {
public:
    virtual void update(float dt) = 0;
protected:
    explicit ComponentSystem(entt::registry<uint32_t>& registry) : registry(registry) {}
    entt::registry<uint32_t>& registry;
};


#endif //NIVEOUS_COMPONENTSYSTEM_H
