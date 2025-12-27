#pragma once

#include "core/system_manager.h"
#include <SDL3/SDL.h>

#include "core/texture_loader.h"
#include "core/events.h"

class World;


class RenderSystem : public System
{
public:
    void init(SDL_Window* window);

    void render();

    SDL_Renderer *renderer;

    TextureLoader textureLoader;

    void onLogicalSizeChanged(LogicalSizeChangedEvent *event);

    void initImGui(SDL_Window * window);
    void renderImGui();

    void onWindowSetSize(WindowResizeRequestedEvent* event);
    

private:

    int windowWidth;
    int windowHeight;

    int logicalWidth;
    int logicalHeight;
};