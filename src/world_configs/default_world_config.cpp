#include "default_world_config.h"
#include "components.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_sdlrenderer3.h>

void DefaultWorldConfig::setupComponents(World *world)
{

    std::cout << "Setting up components in DefaultWorldConfig." << std::endl;
    
    world->registerComponent<RenderComponent>();
    world->registerComponent<Transform>();
    world->registerComponent<PlayerControlComponent>();
    world->registerComponent<VelocityComponent>();
    world->registerComponent<AnimationComponent>();
    world->registerComponent<AnimationControllerComponent>();
    world->registerComponent<ColliderComponent>();
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
    world->subscribeEvent<RenderSystem, WindowResizeRequestedEvent>(renderSystem.get(), &RenderSystem::onWindowSetSize);
    
    Signature signature;
    signature.set(world->getComponentType<RenderComponent>().id, true);
    world->setSystemSignature<RenderSystem>(signature);

    playerControllerSystem = world->registerSystem<PlayerControllerSystem>();

    Signature playerControlSignature;
    playerControlSignature.set(world->getComponentType<PlayerControlComponent>().id, true);
    playerControlSignature.set(world->getComponentType<Transform>().id, true);
    playerControlSignature.set(world->getComponentType<VelocityComponent>().id, true);
    playerControlSignature.set(world->getComponentType<AnimationControllerComponent>().id, true);
    world->setSystemSignature<PlayerControllerSystem>(playerControlSignature);

    playerControllerSystem->init();
    
    movementSystem = world->registerSystem<MovementSystem>();

    Signature movementSignature;
    movementSignature.set(world->getComponentType<Transform>().id, true);
    movementSignature.set(world->getComponentType<VelocityComponent>().id, true);
    world->setSystemSignature<MovementSystem>(movementSignature);


    animationSystem = world->registerSystem<AnimationSystem>();
    Signature animationSignature;
    animationSignature.set(world->getComponentType<AnimationComponent>().id, true);
    animationSignature.set(world->getComponentType<RenderComponent>().id, true);
    world->setSystemSignature<AnimationSystem>(animationSignature);

    world->subscribeEvent<AnimationSystem, EntityChangedStateEvent>(animationSystem.get(), &AnimationSystem::onEntityStateChanged);


    colliderSystem = world->registerSystem<ColliderSystem>();
    Signature colliderSignature;
    colliderSignature.set(world->getComponentType<Transform>().id, true);
    colliderSignature.set(world->getComponentType<ColliderComponent>().id, true);
    world->setSystemSignature<ColliderSystem>(colliderSignature);
    
}

void DefaultWorldConfig::update(float deltaTime)
{

    processSDLEvents();

    imGuiNewFrame();

    playerControllerSystem->update(deltaTime);
    animationSystem->update(deltaTime);
    movementSystem->update(deltaTime);

    colliderSystem->update(deltaTime);
    
    renderSystem->render();
}

void DefaultWorldConfig::shutdown()
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void DefaultWorldConfig::processSDLEvents()
{

    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent))
    {
        windowSystem->processEvent(sdlEvent);
        playerControllerSystem->processEvents(sdlEvent);
        ImGui_ImplSDL3_ProcessEvent(&sdlEvent);
    }
}

void DefaultWorldConfig::imGuiNewFrame()
{
    ImGui_ImplSDL3_NewFrame();
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui::NewFrame();  

    ImGui::ShowDemoWindow();
}
