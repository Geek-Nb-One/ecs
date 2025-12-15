#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <unordered_map>
#include <string>

class TextureLoader
{
public:
    void setRenderer(SDL_Renderer *renderer)
    {
        this->renderer = renderer;
    }

    SDL_Texture *getTexture(const char *filePath)
    {
        if(textures.find(filePath) == textures.end())
        {
            return loadTexture(filePath);
        }
        return textures[filePath];
    }

private:
    SDL_Renderer *renderer;

    std::unordered_map<std::string, SDL_Texture *> textures;

    SDL_Texture *loadTexture(const char *filePath)
    {

        SDL_Surface *surface = IMG_Load(filePath);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);
        textures[filePath] = texture;
        return texture;
    }
};