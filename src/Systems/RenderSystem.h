#pragma once

#include <SDL3/SDL_render.h>
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
#include "../Core/ECS.h"

class RenderSystem : public System
{
public:
    // initializes window
    bool Init();

    // loads media
    bool LoadMedia();

    // Things that happen every frame
    void Update();

    // Renders texture
    void Render(TransformComponent transfromComponent, TextureComponent textureComponent, SDL_FRect* clip = nullptr, double degrees = 0.0, SDL_FPoint* center = nullptr, SDL_FlipMode flipMode = SDL_FLIP_NONE );

    // Closing stuff
    void Close();

    // Loads texture from file
    bool loadTexture(TextureComponent* textureComponent);


private:
    SDL_Renderer* mRenderer = nullptr;
    SDL_Window* mWindow = nullptr;

    SDL_FRect mCamera;
};
