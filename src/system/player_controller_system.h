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

    void init();



private:

    std::array<bool, 4> keyStates = {false, false, false, false}; // W, A, S, D

    SDL_Gamepad * gamepad;


    bool connectGamePad(SDL_JoystickID id);
    void scanGamepad();

    bool gamepadConnected() const ;

    float gamepadAxisDeadZone = 8000.0f;
    glm::vec2 gamepadAxisValue = glm::vec2(0.0f, 0.0f);

};