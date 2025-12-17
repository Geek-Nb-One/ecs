#pragma once

#include "core/system_manager.h"
#include "components.h"

class ColliderSystem : public System
{

public:
    void update(float deltaTime)
    {
        // Collision detection and response logic would go here
        std::vector<Entity> entityList(entities.begin(), entities.end());

        for(int i = 0 ; i < entityList.size(); ++i)
        {
            auto& transformA = world->getComponent<Transform>(entityList[i]);
            auto& colliderA = world->getComponent<ColliderComponent>(entityList[i]);

            SDL_FRect rectA;
            rectA.x = transformA.position.x + colliderA.offset.x;
            rectA.y = transformA.position.y + colliderA.offset.y;
            rectA.w = colliderA.size.x;
            rectA.h = colliderA.size.y;

            for(int j = i + 1 ; j < entityList.size(); ++j)
            {
                auto& transformB = world->getComponent<Transform>(entityList[j]);
                auto& colliderB = world->getComponent<ColliderComponent>(entityList[j]);

                SDL_FRect rectB;
                rectB.x = transformB.position.x + colliderB.offset.x;
                rectB.y = transformB.position.y + colliderB.offset.y;
                rectB.w = colliderB.size.x;
                rectB.h = colliderB.size.y;

                if(SDL_HasRectIntersectionFloat(&rectA, &rectB))
                {
                    // Simple collision response: just print to console for now
                    std::cout << "Collision detected between Entity " << entityList[i].id << " and Entity " << entityList[j].id << std::endl;
                }
            }
        }
    }
};