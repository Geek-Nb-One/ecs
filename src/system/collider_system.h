#pragma once

#include "core/system_manager.h"
#include "components.h"

#include "system/quadtree.h"
#include <set>
#include <vector>

class ColliderSystem : public System
{
public:
    virtual void onAddEntity(Entity entity) override;
    virtual void onRemoveEntity(Entity entity) override;
    void update(float deltaTime);

private:
    Quadtree staticQuadtree{0.0f, 0.0f, 1000.0f, 1000.0f};
    Quadtree dynamicQuadtree{0.0f, 0.0f, 1000.0f, 1000.0f};

    std::vector<Entity> dynamicEntities;

    void addColliderToQuadtree(Entity entity, Quadtree &quadtree);
    void queryColliderToQuadtree(Entity entity, Quadtree &quadtree, std::set<Entity> &outEntities);
    void addCollision(Entity a, Entity b, std::set<std::pair<Entity, Entity>> &handledPairs);

    void resolveCollisions(std::set<std::pair<Entity, Entity>> &handledPairs);


    bool isColliding(const Entity a, const Entity b);
   
    

};