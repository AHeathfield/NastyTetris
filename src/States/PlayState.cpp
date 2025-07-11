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

    mTestBlock = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mTestBlock,
            TextureComponent{
                .texture = nullptr,
                .path = "src/Assets/yellowBlock.png",
            });
    gCoordinator.AddComponent(
            mTestBlock,
            TransformComponent{
                .position = Vector2(920.f, 140.f),
                .rotation = Vector2(),
                .scale = Vector2()
            });
    gCoordinator.AddComponent(
            mTestBlock,
            TetrisGravityComponent{
                .gravity = 40.f
            });
    gCoordinator.AddComponent(
            mTestBlock,
            BoxColliderComponent{
                .position = Vector2(920.f, 140.f),
                .w = 40.f,
                .h = 40.f
            });
    gCoordinator.AddComponent(
            mTestBlock,
            MoveComponent{
                .distance = 40.f
            });


    // These are the bounds for actual game
    mBottomBoundCollider = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBottomBoundCollider,
            BoxColliderComponent{
                .position = Vector2(720.f, 940.f),
                .w = static_cast<float>(1200 - 720),
                .h = 80.f
            });
    gCoordinator.AddComponent(
            mBottomBoundCollider,
            TransformComponent{
                .position = Vector2(720.f, 940.f),
            });


    mRightBoundCollider = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mRightBoundCollider,
            BoxColliderComponent{
                .position = Vector2(1160.f, 100.f),
                .w = 40.f,
                .h = static_cast<float>(980 - 100 - 40)
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
                .w = 40.f,
                .h = static_cast<float>(980 - 100 - 40)
            });
    gCoordinator.AddComponent(
            mLeftBoundCollider,
            TransformComponent{
                .position = Vector2(720.f, 100.f),
            });
    // If I want to see the invis collider
    // gCoordinator.AddComponent(
    //         mLeftBoundCollider,
    //         TextureComponent{
    //             .texture = nullptr,
    //             .spriteClip = SDL_FRect{0.f, 0.f, 40.f, 840.f},
    //             .path = "src/Assets/debug.png",
    //         });

}

void PlayState::Exit()
{
    // Destroy the Texture component
    auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(mBackground);
    textureComponent.destroy();

    gCoordinator.DestroyEntity(mBackground);
}
