#include "core/game_engine.h"
#include <fstream>
#include <iostream>

#include "system/quadtree.h"
#include "core/entity_manager.h"

int main(int, char **)
{


    // Quadtree qt(0.0f, 0.0f, 1000.0f, 1000.0f);

    // qt.insertRectangle(Entity{1}, 100.0f, 100.0f, 50.0f, 50.0f);
    // qt.insertRectangle(Entity{4}, 110.0f, 100.0f, 50.0f, 50.0f);
    // qt.insertRectangle(Entity{5}, 120.0f, 100.0f, 50.0f, 50.0f);
    // qt.insertPoint(Entity{2}, 200.0f, 200.0f);
    // qt.insertCircle(Entity{3}, 300.0f, 300.0f, 25.0f);
    // qt.insertRectangle(Entity{6}, 575.0f, 575.0f, 50.0f, 50.0f);
    
    // qt.debugPrint();

    // qt.insertRectangle(Entity{7}, -50.0f, -50.0f, 50.0f, 50.0f);

    // qt.debugPrint();

    // std::set<Entity> result;
    // qt.queryRectangle(584.0f, 575.0f, 10.0f, 10.0f,result);

    // std::cout << "Query Results:" << std::endl;
    // for(const auto &entity : result)   
    // {
        
    //     std::cout << "Entity ID: " << entity.id << std::endl;
    // }

    std::ofstream logFile("debug.log");
    logFile << "Main started" << std::endl;
    logFile.flush();

    try {
        GameEngine game;
        logFile << "GameEngine created" << std::endl;
        logFile.flush();
        
        game.run();
        
        logFile << "GameEngine exited" << std::endl;
    } catch (const std::exception& e) {
        logFile << "Exception: " << e.what() << std::endl;
    }
    
    logFile.close();

    return 0;
}
