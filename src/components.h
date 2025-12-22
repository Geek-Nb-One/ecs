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


struct AnimationFrame{
    float duration;
    SDL_Texture* texture;
    
    glm::vec2 srcPosition;
    glm::vec2 srcSize;

    glm::vec2 destPosition;
    glm::vec2 destSize;
};

struct AnimationComponent{
    std::vector<AnimationFrame>* frames =nullptr;
    int currentFrameIndex = 0;
    float elapsedTime = 0.0f;
    bool loop = true;
    bool playing = false;

    size_t frameCount() const {
        return frames ? frames->size() : 0;
    }

    bool isEmpty() const {
        if(frames == nullptr) return true;
        return frames->empty();
    }

    AnimationFrame& getCurrentFrame() {
        return (*frames)[currentFrameIndex];
    }
    
};

struct AnimationControllerComponent{
    std::string currentState;
    std::unordered_map<std::string, std::vector<AnimationFrame>> stateAnimations;
};


struct ColliderComponent{


    enum class Shape{
        Rectangle,
        Circle
    } shape = Shape::Rectangle;

    glm::vec2 offset;
    glm::vec2 size; // For rectangle: width and height; For circle: x = radius, y unused    
    

    bool isStatic = false;
    
};