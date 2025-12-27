#pragma once



class World;


class WorldConfig{

    public:
    void setup(World* world);
    virtual void setupComponents(World* world)=0;
    virtual void setupSystems(World* world)=0;
    virtual void update(float deltaTime)=0;
    virtual void shutdown(){};
    virtual ~WorldConfig()=default;
};


#include "core/world.h"


