#include "system/collider_system.h"

#include <algorithm>
#include <iostream>
#include <set>
#include "collider_system.h"

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

    resolveCollisions(handledPairs);

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

void ColliderSystem::resolveCollisions(std::set<std::pair<Entity, Entity>> &handledPairs)
{
    const float RESOLUTION_DELTA = 0.25f; // Small step to push back entities
    const int MAX_ITERATIONS = 100; // Prevent infinite loops

    for (const auto &pair : handledPairs)
    {
        Entity a = pair.first;
        Entity b = pair.second;

        auto &colliderA = world->getComponent<ColliderComponent>(a);
        auto &transformA = world->getComponent<Transform>(a);

        auto &colliderB = world->getComponent<ColliderComponent>(b);
        auto &transformB = world->getComponent<Transform>(b);

        // Check if entities have velocity component (optional, not in signature)
        bool aHasVelocity = world->hasComponent<VelocityComponent>(a);
        bool bHasVelocity = world->hasComponent<VelocityComponent>(b);

        bool aIsMovable = !colliderA.isStatic && aHasVelocity;
        bool bIsMovable = !colliderB.isStatic && bHasVelocity;

        // If neither entity has velocity or both are static, skip resolution
        if (!aIsMovable && !bIsMovable)
        {
            continue;
        }

        // Get velocity directions (normalized)
        glm::vec2 velocityDirA(0.0f, 0.0f);
        glm::vec2 velocityDirB(0.0f, 0.0f);

        if (aIsMovable)
        {
            auto &velocityA = world->getComponent<VelocityComponent>(a);
            float velMagnitude = glm::length(velocityA.velocity);
            if (velMagnitude > 0.001f)
            {
                velocityDirA = glm::normalize(velocityA.velocity) * -1.0f; // Reverse direction
            }
        }

        if (bIsMovable)
        {
            auto &velocityB = world->getComponent<VelocityComponent>(b);
            float velMagnitude = glm::length(velocityB.velocity);
            if (velMagnitude > 0.001f)
            {
                velocityDirB = glm::normalize(velocityB.velocity) * -1.0f; // Reverse direction
            }
        }

        // Push entities back along their velocity direction until they don't overlap
        int iterations = 0;
        while (isColliding(a, b) && iterations < MAX_ITERATIONS)
        {
            if (aIsMovable && bIsMovable)
            {
                // Both entities move, push back equally
                transformA.position.x += velocityDirA.x * RESOLUTION_DELTA;
                transformA.position.y += velocityDirA.y * RESOLUTION_DELTA;

                transformB.position.x += velocityDirB.x * RESOLUTION_DELTA;
                transformB.position.y += velocityDirB.y * RESOLUTION_DELTA;
            }
            else if (aIsMovable)
            {
                // Only A moves
                transformA.position.x += velocityDirA.x * RESOLUTION_DELTA;
                transformA.position.y += velocityDirA.y * RESOLUTION_DELTA;
            }
            else if (bIsMovable)
            {
                // Only B moves
                transformB.position.x += velocityDirB.x * RESOLUTION_DELTA;
                transformB.position.y += velocityDirB.y * RESOLUTION_DELTA;
            }

            iterations++;
        }

        // Stop velocity after collision resolution
        if (aIsMovable)
        {
            auto &velocityA = world->getComponent<VelocityComponent>(a);
            velocityA.velocity = glm::vec2(0.0f, 0.0f);
        }

        if (bIsMovable)
        {
            auto &velocityB = world->getComponent<VelocityComponent>(b);
            velocityB.velocity = glm::vec2(0.0f, 0.0f);
        }
    }
}

bool ColliderSystem::isColliding(const Entity a, const Entity b)
 {
        auto &colliderA = world->getComponent<ColliderComponent>(a);
        auto &transformA = world->getComponent<Transform>(a);

        auto &colliderB = world->getComponent<ColliderComponent>(b);
        auto &transformB = world->getComponent<Transform>(b);

        // Simple AABB collision detection for rectangles
        if (colliderA.shape == ColliderComponent::Shape::Rectangle &&
            colliderB.shape == ColliderComponent::Shape::Rectangle)
        {
            float axMin = transformA.position.x + colliderA.offset.x;
            float ayMin = transformA.position.y + colliderA.offset.y;
            float axMax = axMin + colliderA.size.x;
            float ayMax = ayMin + colliderA.size.y;

            float bxMin = transformB.position.x + colliderB.offset.x;
            float byMin = transformB.position.y + colliderB.offset.y;
            float bxMax = bxMin + colliderB.size.x;
            float byMax = byMin + colliderB.size.y;

            return (axMin < bxMax && axMax > bxMin &&
                    ayMin < byMax && ayMax > byMin);
        }

        // Additional collision detection logic for circles and mixed shapes can be added here

        return false;
    }
