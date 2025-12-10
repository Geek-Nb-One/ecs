#include "game_engine.h"


#include "system/movement_system.h"

void GameEngine::run()
{
    init();
    loop();
}

void GameEngine::init()
{

    createSystems();
}

void GameEngine::createSystems()
{

    createSystem<MovementSystem>();

}

void GameEngine::loop()
{
    std::cout << "Game loop started!" << std::endl;
    for(auto system : systems){
        system->update(0.016); // Assuming a fixed deltaTime for demonstration
    }   
}

void GameEngine::destroy()
{
}
