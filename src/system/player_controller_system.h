#pragma once

#include "core/system_manager.h"
#include "core/events.h"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include "components.h"
#include <array>


class PlayerControllerSystem : public System
{
public:
    void processEvents(SDL_Event &sdlEvent);

    void update(float deltaTime);
private:

    std::array<bool, 4> keyStates = {false, false, false, false}; // W, A, S, D

};