#pragma once

#include "pch.h"
#include "system/system.h"

class GameEngine{

public:

    void run();

private:

    void init();
    void loop();
    void destroy();

    void createSystems();

    std::set<System*, std::less<System*>> systems;

    template <typename T, typename... Args>
    T* createSystem(Args&&... args){
        static_assert(std::is_base_of_v<System, T>, "T must be a subclass of System");
        T* system = new T(std::forward<Args>(args)...);
        systems.insert(system);
        return system;
    }



};