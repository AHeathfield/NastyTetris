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

    bool LoadAllMedia();

    bool LoadMedia(TextureComponent* textureComponent);

    // Things that happen every frame
    void Update();

    // Renders texture
    void Render(TransformComponent transfromComponent, TextureComponent textureComponent, double degrees = 0.0, SDL_FPoint* center = nullptr, SDL_FlipMode flipMode = SDL_FLIP_NONE );

    // TODO: ADD THIS TO CHUCK NORRIS GAME!!
    void EnableTextInput();
    void DisableTextInput();

    // Closing stuff
    void Close();

    // Loads texture from file
    // We use TextureComponent& when we just want handle the object normally but its really big, we use TextureComponent* when we actually want to change the value of the object being passed in
    bool loadTexture(TextureComponent* textureComponent);

    bool loadFromRenderedText(TextureComponent* textureComponent);

private:
    SDL_Renderer* mRenderer = nullptr;
    SDL_Window* mWindow = nullptr;

    SDL_FRect mCamera;
};
