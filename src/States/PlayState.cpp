#include "PlayState.h"

extern Coordinator gCoordinator;

void PlayState::Enter()
{
    mBackground = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBackground,
            TextureComponent{
                .texture = nullptr,
                .path = "src/Assets/gameBG.png",
            });
    gCoordinator.AddComponent(
            mBackground,
            TransformComponent{
                .position = Vector2(),
                .rotation = Vector2(),
                .scale = Vector2()
            });


    // These are the bounds for actual game
    mBottomBoundCollider = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBottomBoundCollider,
            BoxColliderComponent{
                .position = Vector2(720.f, 940.f),
                .w = 1200 - 720,
                .h = 80
            });
    gCoordinator.AddComponent(
            mBottomBoundCollider,
            TransformComponent{
                .position = Vector2(720.f, 940.f),
            });
    gCoordinator.AddComponent(
            mBottomBoundCollider,
            BoundaryComponent{
            });
    // // If I want to see the invis collider
    // gCoordinator.AddComponent(
    //         mBottomBoundCollider,
    //         TextureComponent{
    //             .texture = nullptr,
    //             .spriteClip = SDL_FRect{0.f, 0.f, 480.f, 40.f},
    //             .path = "src/Assets/debug.png",
    //         });


    mRightBoundCollider = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mRightBoundCollider,
            BoxColliderComponent{
                .position = Vector2(1160.f, 100.f),
                .w = 40,
                .h = 980 - 100 - 40
            });
    gCoordinator.AddComponent(
            mRightBoundCollider,
            TransformComponent{
                .position = Vector2(1160.f, 100.f),
            });


    mLeftBoundCollider = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mLeftBoundCollider,
            BoxColliderComponent{
                .position = Vector2(720.f, 100.f),
                .w = 40,
                .h = 980 - 100 - 40
            });
    gCoordinator.AddComponent(
            mLeftBoundCollider,
            TransformComponent{
                .position = Vector2(720.f, 100.f),
            });

    // Test Text
    Entity testText = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            testText,
            TextureComponent{
                .texture = nullptr,
                .path = "src/Assets/8bit16.ttf",
                .isText = true,
                .text = "0"
            });
    gCoordinator.AddComponent(
            testText, 
            TransformComponent{
                .position = Vector2(360.f, 600.f)
            });
}

void PlayState::Exit()
{
    // Destroy the Texture component
    auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(mBackground);
    textureComponent.destroy();

    gCoordinator.DestroyEntity(mBackground);
}
