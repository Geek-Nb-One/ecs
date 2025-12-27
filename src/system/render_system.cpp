#include "render_system.h"
#include "core/world.h"
#include "components.h"
#include <algorithm>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdlrenderer3.h>
#include <imgui/imgui_impl_sdl3.h>

void RenderSystem::init(SDL_Window *window)
{
    renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderVSync(renderer, true);

    textureLoader.setRenderer(renderer);

    initImGui(window);
}

void RenderSystem::render()
{

    std::vector<Entity> sortedEntities;
    std::vector<Entity> colliderEntities;

    for (auto const &entity : getEntities())
    {
        sortedEntities.push_back(entity);
        if (world->hasComponent<ColliderComponent>(entity))
        {
            auto collider = world->getComponent<ColliderComponent>(entity);
            if (collider.renderFrame)
            {
                colliderEntities.push_back(entity);
            }
        }
    }

    std::sort(sortedEntities.begin(), sortedEntities.end(), [this](const Entity &a, const Entity &b)
              {
        auto& transformA = world->getComponent<Transform>(a);
        auto& transformB = world->getComponent<Transform>(b);
        if (transformA.position.z != transformB.position.z) return transformA.position.z < transformB.position.z;
        return transformA.position.y < transformB.position.y; });

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // Rendering code would go here

    for (auto const &entity : sortedEntities)
    {
        // Render each entity
        // This is where you'd retrieve the RenderComponent and draw it
        RenderComponent &renderComp = world->getComponent<RenderComponent>(entity);
        Transform &transform = world->getComponent<Transform>(entity);
        SDL_FRect srcRect = {
            renderComp.srcPosition.x,
            renderComp.srcPosition.y,
            renderComp.srcSize.x,
            renderComp.srcSize.y};
        SDL_FRect destRect = {
            renderComp.destPosition.x + transform.position.x,
            renderComp.destPosition.y + transform.position.y,
            renderComp.destSize.x,
            renderComp.destSize.y};
        SDL_RenderTexture(renderer, renderComp.texture, &srcRect, &destRect);
    }

    for (auto const &entity : colliderEntities)
    {
        auto &collider = world->getComponent<ColliderComponent>(entity);
        auto &transform = world->getComponent<Transform>(entity);

        SDL_FRect rect;
        if (collider.shape == ColliderComponent::Shape::Rectangle)
        {
            rect.x = transform.position.x + collider.offset.x;
            rect.y = transform.position.y + collider.offset.y;
            rect.w = collider.size.x;
            rect.h = collider.size.y;

            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for rectangles
            SDL_RenderRect(renderer, &rect);
        }
        else if (collider.shape == ColliderComponent::Shape::Circle)
        {
            // For simplicity, render circle as a bounding box
            rect.x = transform.position.x + collider.offset.x - collider.size.x;
            rect.y = transform.position.y + collider.offset.y - collider.size.x;
            rect.w = collider.size.x * 2;
            rect.h = collider.size.x * 2;

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for circles
            SDL_RenderRect(renderer, &rect);
        }
    }

    renderImGui();

    SDL_RenderPresent(renderer);
}

void RenderSystem::onLogicalSizeChanged(LogicalSizeChangedEvent *event)
{
    logicalWidth = event->width;
    logicalHeight = event->height;
    
    SDL_SetRenderLogicalPresentation(renderer, logicalWidth, logicalHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);
}

void RenderSystem::initImGui(SDL_Window *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
}

void RenderSystem::renderImGui()
{
    SDL_SetRenderLogicalPresentation(renderer, windowWidth, windowHeight, SDL_LOGICAL_PRESENTATION_DISABLED);
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_SetRenderLogicalPresentation(renderer, logicalWidth, logicalHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);

}

void RenderSystem::onWindowSetSize(WindowResizeRequestedEvent *event)
{
    windowWidth = event->width;
    windowHeight = event->height;
}
