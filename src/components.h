#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>


struct Transform{
    glm::vec3 position ;
    float rotation;
    glm::vec2 scale;

   };

struct RenderComponent{
    SDL_Texture* texture;
    
    glm::vec2 srcPosition;
    glm::vec2 srcSize;

    glm::vec2 destPosition;
    glm::vec2 destSize;

};

struct PlayerControlComponent{

    float speed=100.0f;

};

struct VelocityComponent{
    glm::vec2 velocity;
};
