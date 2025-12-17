#pragma once


#include "core/system_manager.h"

#include "components.h"

class AnimationSystem : public System
{
public:
    void update(float deltaTime)
    {
        // Animation update logic would go here
        for(auto entity : entities)
        {
            animateEntity(entity, deltaTime);
        }
    }

    void onEntityStateChanged(EntityChangedStateEvent* event)
    {
        // Handle entity state change if needed
        if(entities.find(event->entity) != entities.end())
        {
            // For example, reset animation on state change
            auto &anim = world->getComponent<AnimationComponent>(event->entity);
            auto & animController = world->getComponent<AnimationControllerComponent>(event->entity);
            resetAnimation(anim);
            if(animController.stateAnimations.find(event->stateName) != animController.stateAnimations.end())
            {
                anim.frames = &animController.stateAnimations[event->stateName];
            }
            animController.currentState = event->stateName;
        }
    }
private:
    void animateEntity(Entity& entity, float deltaTime)
    {
          auto &anim = world->getComponent<AnimationComponent>(entity);
        auto &render = world->getComponent<RenderComponent>(entity); // assumes you have one


        
        if (!anim.playing || anim.isEmpty()) return;
        
           
        anim.elapsedTime += deltaTime;
        
        while (anim.elapsedTime >= anim.getCurrentFrame().duration) {



            anim.elapsedTime -= anim.getCurrentFrame().duration;
            anim.currentFrameIndex++;

            if (anim.currentFrameIndex >= anim.frameCount()) {
                
                if (anim.loop) anim.currentFrameIndex = 0;
                else { anim.currentFrameIndex = anim.frameCount() - 1; anim.playing = false; }
            }
        }


        // Update render src rect from frame index
        render.texture = anim.getCurrentFrame().texture;

        render.srcPosition = anim.getCurrentFrame().srcPosition;
        render.srcSize = anim.getCurrentFrame().srcSize;

        render.destPosition = anim.getCurrentFrame().destPosition;
        render.destSize = anim.getCurrentFrame().destSize;
    
    }

    void resetAnimation(AnimationComponent& anim)
    {
        anim.currentFrameIndex = 0;
        anim.elapsedTime = 0.0f;
        anim.playing = true;
    }

};