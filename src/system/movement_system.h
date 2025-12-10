#pragma once


#include "system.h"

class MovementSystem : public System {
public:
    void update(double deltaTime) override {
        // Implementation of movement logic
        std::cout << "Updating MovementSystem with deltaTime: " << deltaTime << std::endl;
    }

    Priority getPriority() const override {
        return Priority::MOVEMENT;
    }
};