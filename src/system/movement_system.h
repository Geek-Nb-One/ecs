#pragma once

#include "core/system_manager.h"
#include "components.h"

class MovementSystem : public System
{
public:
    void update(float deltaTime)
    {
        for (auto const &entity : entities)
        {
            auto &transform = world->getComponent<Transform>(entity);
            auto &velocity = world->getComponent<VelocityComponent>(entity);

            transform.position.x += velocity.velocity.x * deltaTime;
            transform.position.y += velocity.velocity.y * deltaTime;
        }
    }
};