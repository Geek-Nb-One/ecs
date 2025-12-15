#pragma once

#include "core/system_manager.h"
#include <SDL3/SDL.h>

#include "core/texture_loader.h"

class World;


class RenderSystem : public System
{
public:
    void init(SDL_Window* window);

    void render();

    SDL_Renderer *renderer;

    TextureLoader textureLoader;

    

};