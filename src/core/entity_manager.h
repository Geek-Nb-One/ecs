#pragma once


#include <set>
#include <array>
#include <stdexcept>
#include <queue>
#include <cassert>
#include <cstdint>
#include <bitset>
#include "core/types.h"

class Entity {
public:
    std::uint32_t id;

    bool operator==(const Entity& other) const {
        return id == other.id;
    }

    bool operator!=(const Entity& other) const {
        return id != other.id;
    }

    bool operator<(const Entity& other) const {
        return id < other.id;
    }
};

namespace std {
    template <>
    struct hash<Entity> {
        std::size_t operator()(const Entity& entity) const {
            return std::hash<std::uint32_t>{}(entity.id);
        }
    };
}



using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager{

    public:
        EntityManager(){
            for (std::uint32_t id = 0; id < MAX_ENTITIES; ++id){
                availableEntities.push(Entity{id});
            }
        }
        Entity CreateEntity(){
            assert(livingEntityCount < MAX_ENTITIES && "Too many entities in existence.");
            Entity id = availableEntities.front();
            availableEntities.pop();
            ++livingEntityCount;
            return id;
        }
        void DestroyEntity(Entity entity){
            assert(entity.id < MAX_ENTITIES && "Entity out of range.");
            signatures[entity.id].reset();
            availableEntities.push(entity);
            --livingEntityCount;
        }
        void SetSignature(Entity entity, Signature signature){
            assert(entity.id < MAX_ENTITIES && "Entity out of range.");
            signatures[entity.id] = signature;
        }

        Signature GetSignature(Entity entity){
            assert(entity.id < MAX_ENTITIES && "Entity out of range.");
            return signatures[entity.id];
        }

    private:
        std::queue<Entity> availableEntities{};
        std::array<Signature, MAX_ENTITIES> signatures{};
        uint32_t livingEntityCount{};


};