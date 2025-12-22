#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <cassert>
#include <array>

#include "core/types.h"

class Entity;

struct ComponentType
{
    std::uint32_t id;
};

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray
{
public:
    void insertData(Entity entity, T component)
    {
        assert(entityToIndexMap.find(entity) == entityToIndexMap.end() && "Component added to same entity more than once.");
        size_t newIndex = size;
        entityToIndexMap[entity] = newIndex;
        indexToEntityMap[newIndex] = entity;
        componentArray[newIndex] = component;
        ++size;
    }

    void removeData(Entity entity)
    {
        assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Removing non-existent component.");
        size_t indexOfRemovedEntity = entityToIndexMap[entity];
        size_t indexOfLastElement = size - 1;
        componentArray[indexOfRemovedEntity] = componentArray[indexOfLastElement];

        Entity entityOfLastElement = indexToEntityMap[indexOfLastElement];
        entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
        indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(indexOfLastElement);

        --size;
    }

    T &getData(Entity entity)
    {
        assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Retrieving non-existent component");
        return componentArray[entityToIndexMap[entity]];
    }

    void EntityDestroyed(Entity entity) override
    {
        // Implementation for handling entity destruction
        if (entityToIndexMap.find(entity) != entityToIndexMap.end())
        {
            removeData(entity);
        }
    }

private:
    std::unordered_map<Entity, size_t> entityToIndexMap{};
    std::unordered_map<size_t, Entity> indexToEntityMap{};
    std::array<T, MAX_ENTITIES> componentArray{};
    size_t size{};
};

class ComponentManager
{
public:
    template <typename T>
    void RegisterComponent()
    {

        assert(componentTypes.find(typeid(T)) == componentTypes.end() && "Registering component type more than once.");

        componentTypes[typeid(T)] = ComponentType{nextComponentType};
        componentArrays[typeid(T)] = std::make_shared<ComponentArray<T>>();
        ++nextComponentType;
    }

    template <typename T>
    ComponentType GetComponentType()
    {
        std::type_index typeIndex = std::type_index(typeid(T));
        assert(componentTypes.find(typeIndex) != componentTypes.end() && "Component not registered before use.");

        return componentTypes[typeIndex];
    }

    template <typename T>
    void AddComponent(Entity entity, T component)
    {
        GetComponentArray<T>()->insertData(entity, component);
    }

    template <typename T>
    void RemoveComponent(Entity entity)
    {
        GetComponentArray<T>()->removeData(entity);
    }

    template <typename T>
    T &GetComponent(Entity entity)
    {
        return GetComponentArray<T>()->getData(entity);
    }

    void EntityDestroyed(Entity entity)
    {
        for (auto const &pair : componentArrays)
        {
            auto const &componentArray = pair.second;
            componentArray->EntityDestroyed(entity);
        }
    }

private:
    std::unordered_map<std::type_index, ComponentType> componentTypes{};
    std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> componentArrays{};
    std::uint32_t nextComponentType = 0;

    template <typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray()
    {
        std::type_index typeIndex = std::type_index(typeid(T));
        assert(componentTypes.find(typeIndex) != componentTypes.end() && "Component not registered before use.");

        return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeIndex]);
    }
};