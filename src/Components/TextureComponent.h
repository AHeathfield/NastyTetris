#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>

struct TextureComponent
{
    // I dont think I need this
    // static constexpr float originalSize = -1.f;

    // Contains texture data
    SDL_Texture* texture;
    std::string path;

    // Texture Dimensions
    int width;
    int height;
};
