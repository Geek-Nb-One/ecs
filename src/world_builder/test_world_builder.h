#pragma once


#include "world_builder.h"


class TestWorldBuilder: public WorldBuilder
{

public:
        void buildWorld(World *world) override;


private:

    void buildPlayer(World *world);
    void buildEnvironment(World *world);

};