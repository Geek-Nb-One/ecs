#pragma once

#include "core/system_manager.h"
#include "core/events.h"
#include "render_system.h"

#include <iostream>

#include <SDL3/SDL.h>

class WindowSystem : public System
{
public:
    void init()
    {

        world->subscribeEvent<WindowSystem, WindowResizeRequestedEvent>(this,&WindowSystem::onWindowResizeRequested);
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Game", 800, 600, 0);


    }

    void onWindowResizeRequested(WindowResizeRequestedEvent *event)
    {
        std::cout << "WindowSystem received WindowResizeRequestedEvent: "
                  << "New size = " << event->width << "x" << event->height << std::endl;
        SDL_SetWindowSize(window, event->width, event->height);
    }

    void processEvent(SDL_Event &sdlEvent)
    {
        if (sdlEvent.type == SDL_EVENT_QUIT)
        {
            world->publishEvent<QuitEvent>();
        }
    }

    void destroy(QuitEvent *event)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void initRenderer(RenderSystem &renderSystem)
    {
        renderSystem.init(window);
    }

private:
    SDL_Window *window;
    int width, height;

};