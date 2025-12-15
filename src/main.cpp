#include "core/game_engine.h"
#include <fstream>
#include <iostream>


int main(int, char **)
{


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
