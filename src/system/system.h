#pragma once


#include "../pch.h"

enum class Priority{
    MOVEMENT = 0,
    MEDIUM = 1,
    HIGH = 2
};

class System{


public:
    virtual void update(double deltaTime) = 0;
    virtual Priority getPriority() const = 0;

    bool operator<(const System& other) const {
        return getPriority() < other.getPriority();
    }


};