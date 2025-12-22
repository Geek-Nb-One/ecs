#include "render_system.h"
#include "core/world.h"
#include "components.h"
#include <algorithm>

void RenderSystem::init(SDL_Window *window)
{
    renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderVSync(renderer, true);   

    textureLoader.setRenderer(renderer);
}


void RenderSystem::render()
{

    std::vector<Entity> sortedEntities;

    for (auto const& entity : getEntities())
    {
        sortedEntities.push_back(entity);
    }

    std::sort(sortedEntities.begin(), sortedEntities.end(), [this](const Entity& a, const Entity& b) {
        auto& transformA = world->getComponent<Transform>(a);
        auto& transformB = world->getComponent<Transform>(b);
        if (transformA.position.z != transformB.position.z) return transformA.position.z < transformB.position.z;
        return transformA.position.y < transformB.position.y;
    });


    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // Rendering code would go here

    for (auto const& entity : sortedEntities)
    {
        // Render each entity
        // This is where you'd retrieve the RenderComponent and draw it
        RenderComponent& renderComp = world->getComponent<RenderComponent>(entity);
        Transform& transform = world->getComponent<Transform>(entity);
        SDL_FRect srcRect = {
            renderComp.srcPosition.x,
            renderComp.srcPosition.y,
            renderComp.srcSize.x,
            renderComp.srcSize.y
        };
        SDL_FRect destRect = {
            renderComp.destPosition.x + transform.position.x,
            renderComp.destPosition.y + transform.position.y,
            renderComp.destSize.x,
            renderComp.destSize.y
        };
        SDL_RenderTexture(renderer, renderComp.texture, &srcRect, &destRect);

    }

    SDL_RenderPresent(renderer);
}

void RenderSystem::onLogicalSizeChanged(LogicalSizeChangedEvent *event)
{
    SDL_SetRenderLogicalPresentation(renderer, event->width, event->height,SDL_LOGICAL_PRESENTATION_LETTERBOX);
}
