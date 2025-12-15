#include "render_system.h"

#include "core/world.h"

#include "components.h"

void RenderSystem::init(SDL_Window *window)
{
    renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderVSync(renderer, 1);   

    textureLoader.setRenderer(renderer);
}



void RenderSystem::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // Rendering code would go here

    for (auto const& entity : entities)
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
