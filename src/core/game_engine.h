#pragma once


#include "core/world.h"

#include "core/events.h"




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

    bool isRunning = true;



};