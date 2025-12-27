#include "game_engine.h"

#include "core/events.h"
#include "system/window_system.h"
#include <chrono>
#include <SDL3/SDL.h>

#include "components.h"

#include "world_configs/default_world_config.h"


void GameEngine::run()
{
    init();
    loop();
    shutdown();
}

void GameEngine::onQuitEvent(QuitEvent *event)
{
    isRunning = false;
}

void GameEngine::init()
{
    std::cerr << "Initializing Game Engine..." << std::endl;

    world = new World(new DefaultWorldConfig());

    world->subscribeEvent<GameEngine, QuitEvent>(this, &GameEngine::onQuitEvent);
    
    worldBuilder->buildWorld(world); 

    // world->publishEvent<WindowFullScreenRequestedEvent>(true);
    world->publishEvent<WindowResizeRequestedEvent>(1536, 864);
    world->publishEvent<LogicalSizeChangedEvent>(768, 432);

}

void GameEngine::loop()
{

    std::cerr << "Entering main loop." << std::endl;

    std::chrono::high_resolution_clock::time_point loopStartTime = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point frameStartTime = std::chrono::high_resolution_clock::now();

    while (isRunning)
    {
        std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - frameStartTime).count();
        frameStartTime = currentTime;
        if (world)
        {
            world->update(deltaTime);
        }
        else
        {
            std::cerr << "World pointer is null!" << std::endl;
        }
    }
}

void GameEngine::shutdown()
{
    std::cerr << "Shutting down Game Engine." << std::endl;

    world->shutdown();

}
