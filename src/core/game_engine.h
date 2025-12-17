#pragma once


#include "core/world.h"

#include "core/events.h"

#include "world_builder/test_world_builder.h"



class GameEngine
{

public:
    void run();

    void onQuitEvent(QuitEvent* event);
private:
    void init();
    void loop();
    void shutdown();

    World* world = nullptr;
    WorldBuilder * worldBuilder = new TestWorldBuilder();

    bool isRunning = true;



};