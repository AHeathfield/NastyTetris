#include "RenderSystem.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <string>


extern Coordinator gCoordinator;

bool RenderSystem::Init()
{
    int screenWidth = 1920;
    int screenHeight = 1080;

    // TODO: Don't hardcode this...
    // Only want to use camera as clip when dealing with bg
    mCamera.x = 0.f;
    mCamera.y = 0.f;
    mCamera.w = screenWidth;
    mCamera.h = screenHeight;

    // Framerate Stuff
    // mScreenFps = 60;
    // capTimer = 
 

    // Initializing Window and Renderer
    if (SDL_CreateWindowAndRenderer( "NastyTetris", screenWidth, screenHeight, 0, &mWindow, &mRenderer ) == false )
    {
        SDL_Log( "Window could not be created! SDL error: %s\n", SDL_GetError() );
        return false;
    }

    // Everything initialize good
    return true;
}


void RenderSystem::Update()
{
    // TODO: Bound camera

    // Fills the background
    SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF,  0xFF );
    SDL_RenderClear( mRenderer );

    // Render stuff
    for (const auto& entity : mEntities)
    {
        const auto& transformComponent = gCoordinator.GetComponent<TransformComponent>(entity);
        const auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(entity);

        // WONT USE CAMERA YET since tetris we don't need to follow hahaha
        Render(transformComponent, textureComponent);
    }

    // Update Screen
    SDL_RenderPresent(mRenderer);
 
    // TODO: Do the framerate stuff
}

bool RenderSystem::LoadMedia()
{
    // Initializing Textures
    // std::string log = std::to_string(mEntities.size());
    // const char* logc = log.c_str();
    // SDL_Log(logc);
    for (const auto& entity : mEntities)
    {
        auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(entity);

        // Loading Textures
        if (loadTexture(&textureComponent) == false)
        {
            SDL_Log("Unable to load a texture (put better error msg here lol).\n");
            return false;
        }
    }
    return true;
}


void RenderSystem::Render(TransformComponent transfromComponent, TextureComponent textureComponent, double degrees, SDL_FPoint* center, SDL_FlipMode flipMode)
{
    SDL_FRect dstRect{ transfromComponent.position.x, transfromComponent.position.y, static_cast<float>(textureComponent.width), static_cast<float>(textureComponent.height) };

    //Default to clip dimensions if clip is given
    if (textureComponent.isSpriteClip())
    {
        dstRect.w = textureComponent.spriteClip.w;
        dstRect.h = textureComponent.spriteClip.h;

        // Render Texture on screen
        SDL_RenderTextureRotated(mRenderer, textureComponent.texture, &textureComponent.spriteClip, &dstRect, degrees, center, flipMode );
    }
    else
    {
        // Render Texture on screen
        SDL_RenderTextureRotated(mRenderer, textureComponent.texture, nullptr, &dstRect, degrees, center, flipMode );
    }

}


void RenderSystem::Close()
{
    // Destroys all textures
    for (const auto& entity : mEntities)
    {
        auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(entity);
        textureComponent.destroy();
    }

    // Destroys window
    SDL_DestroyRenderer(mRenderer);
    mRenderer = nullptr;
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;
}

bool RenderSystem::loadTexture(TextureComponent* textureComponent)
{
    // TODO: Clean up texture if already exists
    textureComponent->destroy();
    // SDL_Log(textureComponent.path.c_str());

    //Load surface
    if( SDL_Surface* loadedSurface = IMG_Load( textureComponent->path.c_str() ); loadedSurface == nullptr )
    {
        SDL_Log( "Unable to load image %s! SDL_image error: %s\n", textureComponent->path.c_str(), SDL_GetError() );
    }
    else
    {
        // SDL_Log("Image loaded correctly!");
        //Color key image
        if( SDL_SetSurfaceColorKey( loadedSurface, true, SDL_MapSurfaceRGB( loadedSurface, 0x00, 0xFF, 0xFF ) ) == false )
        {
            SDL_Log( "Unable to color key! SDL error: %s", SDL_GetError() );
        }
        else
        {
            //Create texture from surface
            if( textureComponent->texture = SDL_CreateTextureFromSurface( mRenderer, loadedSurface ); textureComponent->texture == nullptr )
            {
                SDL_Log( "Unable to create texture from loaded pixels! SDL error: %s\n", SDL_GetError() );
            }
            else
            {
                //Get image dimensions
                textureComponent->width = loadedSurface->w;
                textureComponent->height = loadedSurface->h;

            }
        }
        
        //Clean up loaded surface
        SDL_DestroySurface( loadedSurface );
    }

    //Return success if texture loaded
    return textureComponent->texture != nullptr;
}
