#pragma once

#include <SDL3/SDL.h>
#include "../Core/Vector2.h"
#include <SDL3_image/SDL_image.h>
#include <string>

struct TextureComponent
{
    // Contains texture data
    SDL_Texture* texture;
    SDL_FRect spriteClip{-1.f, -1.f, -1.f, -1.f}; // Default values
    std::string path;

    // Texture Dimensions
    int width = 0;
    int height = 0;

    bool isSpriteClip()
    {
        return !(spriteClip.x < 0 || spriteClip.y < 0 || spriteClip.h < 0 || spriteClip.w < 0);
    }

    // Function to destroy the texture properly
    void destroy()
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
        width = 0;
        height = 0;
    }
};
