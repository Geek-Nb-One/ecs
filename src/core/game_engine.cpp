#include "game_engine.h"

#include "core/events.h"
#include "system/window_system.h"
#include <chrono>
#include <SDL3/SDL.h>

#include "components.h"

#include "world_configs/default_world_config.h"

void GameEngine::run()
{
    init();
    loop();
    shutdown();
}

void GameEngine::onQuitEvent(QuitEvent *event)
{
    isRunning = false;
}

void GameEngine::init()
{
    std::cerr << "Initializing Game Engine..." << std::endl;

    world = new World(new DefaultWorldConfig());

    world->subscribeEvent<GameEngine, QuitEvent>(this, &GameEngine::onQuitEvent);
    
    SDL_Texture * charTexture = world->getTexture("F:\\Users\\User\\Documents\\game_studio\\projects\\ecs\\assets\\arts\\gfx\\character.png");
    Entity entity = world->createEntity();
    RenderComponent renderComponent;
    renderComponent.texture = charTexture;
    renderComponent.srcPosition = glm::vec2(0.0f, 0.0f);
    renderComponent.srcSize = glm::vec2(16.0f, 32.0f);
    renderComponent.destPosition = glm::vec2(0.0f, 0.0f);
    renderComponent.destSize = glm::vec2(16.0f, 32.0f);
    world->addComponent<RenderComponent>(entity, renderComponent);

    AnimationComponent animationComponent;
    animationComponent.playing = true;
    animationComponent.loop = true;
    animationComponent.currentFrameIndex = 0;
    animationComponent.elapsedTime = 0.0f;
    
    AnimationControllerComponent animController;
    animController.stateAnimations["walk_down"] =
    {
        // Add frames here
        AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
         AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(16.0f, 0.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
         AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(32.0f, 0.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
         AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(48.0f, 0.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
    };


    animController.stateAnimations["walk_up"] =
    {
        // Add frames here
        AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(0.0f, 64.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
         AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(16.0f, 64.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
         AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(32.0f, 64.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
         AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(48.0f, 64.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
    };

    animController.stateAnimations["walk_right"] =
    {
        // Add frames here
        AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(0.0f, 32.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
         AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(16.0f, 32.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
         AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(32.0f, 32.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
         AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(48.0f, 32.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
    };
    

    animController.stateAnimations["walk_left"] =
    {
        // Add frames here
        AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(0.0f, 96.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
         AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(16.0f, 96.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
         AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(32.0f, 96.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
         AnimationFrame{
            0.1f,
            charTexture,
            glm::vec2(48.0f, 96.0f),
            glm::vec2(16.0f, 32.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(16.0f, 32.0f)
        },
    };
    
    
    
    world->addComponent<AnimationComponent>(entity, animationComponent);
    world->addComponent<AnimationControllerComponent>(entity, animController);

    Transform transform;
    transform.position = glm::vec3(100.0f, 100.0f, 0.0f);
    transform.rotation = 0.0f;
    transform.scale = glm::vec2(1.0f, 1.0f);
    world->addComponent<Transform>(entity, transform);

    PlayerControlComponent playerControlComponent;
    playerControlComponent.speed = 200.0f;
    world->addComponent<PlayerControlComponent>(entity, playerControlComponent);

    VelocityComponent velocityComponent;
    velocityComponent.velocity = glm::vec2(0.0f, 0.0f);
    world->addComponent<VelocityComponent>(entity, velocityComponent);

    std::cerr << "Game Engine initialized." << std::endl;

    // world->publishEvent<WindowFullScreenRequestedEvent>(true);
    world->publishEvent<WindowResizeRequestedEvent>(768, 432);
    world->publishEvent<LogicalSizeChangedEvent>(768, 432);
}

void GameEngine::loop()
{

    std::cerr << "Entering main loop." << std::endl;

    std::chrono::high_resolution_clock::time_point loopStartTime = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point frameStartTime = std::chrono::high_resolution_clock::now();

    while (isRunning)
    {
        std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - frameStartTime).count();
        frameStartTime = currentTime;
        if (world)
        {
            world->update(deltaTime);
        }
        else
        {
            std::cerr << "World pointer is null!" << std::endl;
        }
    }
}

void GameEngine::shutdown()
{
    std::cerr << "Shutting down Game Engine." << std::endl;
}
