#include "default_world_config.h"
#include "components.h"



void DefaultWorldConfig::setupComponents(World *world)
{

    std::cout << "Setting up components in DefaultWorldConfig." << std::endl;
    
    world->registerComponent<RenderComponent>();
    world->registerComponent<Transform>();
    world->registerComponent<PlayerControlComponent>();
    world->registerComponent<VelocityComponent>();
}

void DefaultWorldConfig::setupSystems(World *world)
{
    std::cout << "Setting up systems in DefaultWorldConfig." << std::endl;
    windowSystem = world->registerSystem<WindowSystem>();
    windowSystem->init();

    world->subscribeEvent<WindowSystem, QuitEvent>(windowSystem.get(), &WindowSystem::destroy);
    world->subscribeEvent<WindowSystem, WindowFullScreenRequestedEvent>(windowSystem.get(), &WindowSystem::onWindowFullScreenRequested);

    renderSystem = world->registerSystem<RenderSystem>();
    windowSystem->initRenderer(*renderSystem);

    world->setRenderer(renderSystem->renderer);


    world->subscribeEvent<RenderSystem, LogicalSizeChangedEvent>(renderSystem.get(), &RenderSystem::onLogicalSizeChanged);
    
    Signature signature;
    signature.set(world->getComponentType<RenderComponent>().id, true);
    world->setSystemSignature<RenderSystem>(signature);

    playerControllerSystem = world->registerSystem<PlayerControllerSystem>();

    Signature playerControlSignature;
    playerControlSignature.set(world->getComponentType<PlayerControlComponent>().id, true);
    playerControlSignature.set(world->getComponentType<Transform>().id, true);
    playerControlSignature.set(world->getComponentType<VelocityComponent>().id, true);
    world->setSystemSignature<PlayerControllerSystem>(playerControlSignature);

    movementSystem = world->registerSystem<MovementSystem>();

    Signature movementSignature;
    movementSignature.set(world->getComponentType<Transform>().id, true);
    movementSignature.set(world->getComponentType<VelocityComponent>().id, true);
    world->setSystemSignature<MovementSystem>(movementSignature);
}

void DefaultWorldConfig::update(float deltaTime)
{

    processSDLEvents();

    playerControllerSystem->update(deltaTime);
    movementSystem->update(deltaTime);

    renderSystem->render();
}

void DefaultWorldConfig::processSDLEvents()
{

    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent))
    {
        windowSystem->processEvent(sdlEvent);
        playerControllerSystem->processEvents(sdlEvent);
    }
}
