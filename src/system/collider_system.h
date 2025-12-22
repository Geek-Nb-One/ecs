#pragma once

#include "core/system_manager.h"
#include "components.h"

#include "system/quadtree.h"
#include <algorithm>

class ColliderSystem : public System
{

public:
    virtual void onAddEntity(Entity entity) override
    {
        auto &collider = world->getComponent<ColliderComponent>(entity);
        auto &transform = world->getComponent<Transform>(entity);

        if (collider.isStatic)
        {
            addColliderToQuadtree(entity, staticQuadtree);
        }
        else
        {
            dynamicEntities.push_back(entity);
        }
    }

    virtual void onRemoveEntity(Entity entity) override
    {

      
        staticQuadtree.remove(entity);
    
        dynamicQuadtree.remove(entity);

        auto it = std::find(dynamicEntities.begin(), dynamicEntities.end(), entity);
        if (it != dynamicEntities.end())
        {
            dynamicEntities.erase(it);
        }
    }

    void update(float deltaTime)
    {

        dynamicQuadtree.clear();
        for (const auto &entity : dynamicEntities)
        {
            addColliderToQuadtree(entity, dynamicQuadtree);
        }
        std::set<std::pair<Entity, Entity>> handledPairs;

        for (auto entity : dynamicEntities)
        {
            // Handle dynamic collider collisions here
            // For example, query potential collisions from both static and dynamic quadtrees
            std::set<Entity> potentialCollisions;
            auto &collider = world->getComponent<ColliderComponent>(entity);
            auto &transform = world->getComponent<Transform>(entity);

            queryColliderToQuadtree(entity, staticQuadtree, potentialCollisions);
            queryColliderToQuadtree(entity, dynamicQuadtree, potentialCollisions);

            for (auto otherEntity : potentialCollisions)
            {
                if (otherEntity == entity)
                    continue;
                // Add collision handling logic here
                addCollision(entity, otherEntity, handledPairs);
            }
        }

        for (auto &pair : handledPairs)
        {
            // Process collision between pair.first and pair.second
            // For example, you can publish a collision event here

            std::cout << "Collision detected between Entity " << pair.first.id << " and Entity " << pair.second.id << std::endl;
        }
    }

private:
    Quadtree staticQuadtree{0.0f, 0.0f, 1000.0f, 1000.0f};
    Quadtree dynamicQuadtree{0.0f, 0.0f, 1000.0f, 1000.0f};

    std::vector<Entity> dynamicEntities;

    void addColliderToQuadtree(Entity entity, Quadtree &quadtree)
    {
        auto &collider = world->getComponent<ColliderComponent>(entity);
        auto &transform = world->getComponent<Transform>(entity);

        if (collider.shape == ColliderComponent::Shape::Rectangle)
        {
            quadtree.insertRectangle(entity, transform.position.x + collider.offset.x, transform.position.y + collider.offset.y, collider.size.x, collider.size.y);
        }
        else if (collider.shape == ColliderComponent::Shape::Circle)
        {
            quadtree.insertCircle(entity, transform.position.x + collider.offset.x, transform.position.y + collider.offset.y, collider.size.x);
        }
    }

    void queryColliderToQuadtree(Entity entity, Quadtree &quadtree, std::set<Entity> &outEntities)
    {
        auto &collider = world->getComponent<ColliderComponent>(entity);
        auto &transform = world->getComponent<Transform>(entity);

        if (collider.shape == ColliderComponent::Shape::Rectangle)
        {
            quadtree.queryRectangle(transform.position.x + collider.offset.x, transform.position.y + collider.offset.y, collider.size.x, collider.size.y,outEntities);
        }
        else if (collider.shape == ColliderComponent::Shape::Circle)
        {
            quadtree.queryCircle(transform.position.x + collider.offset.x, transform.position.y + collider.offset.y, collider.size.x,outEntities);
        }
    }

    void addCollision(Entity a, Entity b, std::set<std::pair<Entity, Entity>> &handledPairs)
    {
        // Handle collision between entities a and b
        if (a.id > b.id)
            std::swap(a, b);
        handledPairs.insert(std::make_pair(a, b));
    }
};