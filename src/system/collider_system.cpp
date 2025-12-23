#include "system/collider_system.h"

#include <algorithm>
#include <iostream>
#include <set>

void ColliderSystem::onAddEntity(Entity entity)
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

void ColliderSystem::onRemoveEntity(Entity entity)
{
    staticQuadtree.remove(entity);
    dynamicQuadtree.remove(entity);

    auto it = std::find(dynamicEntities.begin(), dynamicEntities.end(), entity);
    if (it != dynamicEntities.end())
    {
        dynamicEntities.erase(it);
    }
}

void ColliderSystem::update(float /*deltaTime*/)
{
    dynamicQuadtree.clear();
    for (const auto &entity : dynamicEntities)
    {
        addColliderToQuadtree(entity, dynamicQuadtree);
    }

    std::set<std::pair<Entity, Entity>> handledPairs;

    for (auto entity : dynamicEntities)
    {
        std::set<Entity> potentialCollisions;
        auto &collider = world->getComponent<ColliderComponent>(entity);
        auto &transform = world->getComponent<Transform>(entity);

        queryColliderToQuadtree(entity, staticQuadtree, potentialCollisions);
        queryColliderToQuadtree(entity, dynamicQuadtree, potentialCollisions);

        for (auto otherEntity : potentialCollisions)
        {
            if (otherEntity == entity)
                continue;
            addCollision(entity, otherEntity, handledPairs);
        }
    }

    for (auto &pair : handledPairs)
    {
        std::cout << "Collision detected between Entity " << pair.first.id
                  << " and Entity " << pair.second.id << std::endl;
    }
}

void ColliderSystem::addColliderToQuadtree(Entity entity, Quadtree &quadtree)
{
    auto &collider = world->getComponent<ColliderComponent>(entity);
    auto &transform = world->getComponent<Transform>(entity);

    if (collider.shape == ColliderComponent::Shape::Rectangle)
    {
        quadtree.insertRectangle(entity,
                                 transform.position.x + collider.offset.x,
                                 transform.position.y + collider.offset.y,
                                 collider.size.x,
                                 collider.size.y);
    }
    else if (collider.shape == ColliderComponent::Shape::Circle)
    {
        quadtree.insertCircle(entity,
                              transform.position.x + collider.offset.x,
                              transform.position.y + collider.offset.y,
                              collider.size.x);
    }
}

void ColliderSystem::queryColliderToQuadtree(Entity entity, Quadtree &quadtree, std::set<Entity> &outEntities)
{
    auto &collider = world->getComponent<ColliderComponent>(entity);
    auto &transform = world->getComponent<Transform>(entity);

    if (collider.shape == ColliderComponent::Shape::Rectangle)
    {
        quadtree.queryRectangle(transform.position.x + collider.offset.x,
                                transform.position.y + collider.offset.y,
                                collider.size.x,
                                collider.size.y,
                                outEntities);
    }
    else if (collider.shape == ColliderComponent::Shape::Circle)
    {
        quadtree.queryCircle(transform.position.x + collider.offset.x,
                             transform.position.y + collider.offset.y,
                             collider.size.x,
                             outEntities);
    }
}

void ColliderSystem::addCollision(Entity a, Entity b, std::set<std::pair<Entity, Entity>> &handledPairs)
{
    if (a.id > b.id)
        std::swap(a, b);
    handledPairs.insert(std::make_pair(a, b));
}
