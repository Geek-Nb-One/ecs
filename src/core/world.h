#pragma once

#include "core/system_manager.h"
#include "core/entity_manager.h"
#include "core/component_manager.h"
#include "core/event_manager.h"
#include <memory>
#include "core/world_config.h"
#include "core/texture_loader.h"

class World 
{

public:

    World(WorldConfig* worldConfig):worldConfig(worldConfig)
    {
        worldConfig->setup(this);
    }

    void upate(float deltaTime)
    {
        worldConfig->update(deltaTime);
    }


    Entity createEntity()
    {
        return entityManager.CreateEntity();
    }

    void destroyEntity(Entity entity)
    {
        entityManager.DestroyEntity(entity);
        componentManager.EntityDestroyed(entity);
        systemManager.entityDestroyed(entity);
    }

    template<typename EventType>
    void registerComponent()
    {
        componentManager.RegisterComponent<EventType>();
    }
    template<typename T>
    T& addComponent(Entity entity, T component)
    {
        componentManager.AddComponent<T>(entity, component);
        Signature signature = entityManager.GetSignature(entity);
        signature.set(componentManager.GetComponentType<T>().id, true);
        entityManager.SetSignature(entity, signature);
        systemManager.entitySignatureChanged(entity, signature);
        return componentManager.GetComponent<T>(entity);
    }

    template<typename T>
    void removeComponent(Entity entity)
    {
        componentManager.RemoveComponent<T>(entity);
        Signature signature = entityManager.GetSignature(entity);
        signature.set(componentManager.GetComponentType<T>().id, false);
        entityManager.SetSignature(entity, signature);
        systemManager.entitySignatureChanged(entity, signature);
    }

    template<typename T>
    T& getComponent(Entity entity)
    {
        return componentManager.GetComponent<T>(entity);
    }

    template<typename Component>
    ComponentType getComponentType()
    {
        return componentManager.GetComponentType<Component>();
    }

    template<typename T>
    std::shared_ptr<T> registerSystem()
    {
        return systemManager.registerSystem<T>(std::shared_ptr<World>(this));
    }
    template<typename T>
    void setSystemSignature(Signature signature)
    {
        systemManager.setSignature<T>(signature);
    }

    template<typename EventType, typename ... Args>
    void publishEvent(Args&&... args)
    {
        eventManagerPtr->publish<EventType>(std::forward<Args>(args)...);
    }

    template <typename T,typename EventType>
    void subscribeEvent(T* instance, void (T::*memberFunction)(EventType *))
    {
        eventManagerPtr->subscribe<T,EventType>(instance,memberFunction);
    }

    SDL_Texture * getTexture(const char *filePath)
    {
        return textureLoader.getTexture(filePath);
    }

    void update(float deltaTime)
    {
        worldConfig->update(deltaTime);
    }

    void setRenderer(SDL_Renderer* renderer)
    {
        textureLoader.setRenderer(renderer);
    }

private:
    std::shared_ptr<EventManager> eventManagerPtr = std::make_shared<EventManager>();
    EventManager eventManager;
    SystemManager systemManager;
    EntityManager entityManager;
    ComponentManager componentManager;

    WorldConfig* worldConfig;
    TextureLoader textureLoader;
};