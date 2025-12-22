#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <cassert>

#include <set>
#include <functional>

#include "core/event_manager.h"
#include "core/entity_manager.h"

class World;

class System
{
public:
    using iterator = std::set<Entity>::iterator;
    using const_iterator = std::set<Entity>::const_iterator;

    iterator begin() { return entities.begin(); }
    iterator end() { return entities.end(); }
    const_iterator begin() const { return entities.begin(); }
    const_iterator end() const { return entities.end(); }

    const std::set<Entity> &getEntities() const { return entities; }

    void addEntity(Entity entity)
    {
        onAddEntity(entity);
        entities.insert(entity);
    }

    void removeEntity(Entity entity)
    {
        entities.erase(entity);
        onRemoveEntity(entity);
    }

    virtual void onAddEntity(Entity entity) {}
    virtual void onRemoveEntity(Entity entity) {}

    std::shared_ptr<World> world;

    void setWorld(std::shared_ptr<World> world)
    {
        this->world = world;
    }

private:
    std::set<Entity> entities;
};

class SystemManager
{

public:
    template <typename EventType>
    std::shared_ptr<EventType> registerSystem(std::shared_ptr<World> world)
    {
        std::type_index typeIndex = std::type_index(typeid(EventType));
        assert(systems.find(typeIndex) == systems.end() && "Registering system more than once.");
        auto system = std::make_shared<EventType>();
        system->setWorld(world);
        systems[typeIndex] = system;
        return system;
    }

    template <typename EventType>
    void setSignature(Signature signature)
    {
        std::type_index typeIndex = std::type_index(typeid(EventType));
        assert(systems.find(typeIndex) != systems.end() && "System used before registered.");

        signatures[typeIndex] = signature;
    }

    void entityDestroyed(Entity entity)
    {
        for (auto const &pair : systems)
        {
            auto const &system = pair.second;
            system->removeEntity(entity);
        }
    }

    void entitySignatureChanged(Entity entity, Signature entitySignature)
    {
        for (auto const &pair : systems)
        {
            auto const &typeIndex = pair.first;
            auto const &system = pair.second;
            auto const &systemSignature = signatures[typeIndex];

            if ((entitySignature & systemSignature) == systemSignature)
            {
                system->addEntity(entity);
            }
            else
            {
                system->removeEntity(entity);
            }
        }
    }

private:
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems{};
    std::unordered_map<std::type_index, Signature> signatures{};
};

#include "core/world.h"