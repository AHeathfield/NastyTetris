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
    // If I want to see the invis collider
    gCoordinator.AddComponent(
            mBottomBoundCollider,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 480.f, 40.f},
                .path = "src/Assets/debug.png",
            });


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
    // If I want to see the invis collider
    gCoordinator.AddComponent(
            mLeftBoundCollider,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 40.f, 840.f},
                .path = "src/Assets/debug.png",
            });
    //


    // These are the shapes!
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
                .w = 40,
                .h = 40
            });
    gCoordinator.AddComponent(
            mTestBlock,
            MoveComponent{
                .distance = 40.f
            });
    //
    // mTestBlock2 = gCoordinator.CreateEntity();
    // gCoordinator.AddComponent(
    //         mTestBlock2,
    //         TextureComponent{
    //             .texture = nullptr,
    //             .path = "src/Assets/yellowBlock.png",
    //         });
    // gCoordinator.AddComponent(
    //         mTestBlock2,
    //         TransformComponent{
    //             .position = Vector2(960.f, 140.f),
    //             .rotation = Vector2(),
    //             .scale = Vector2()
    //         });
    // gCoordinator.AddComponent(
    //         mTestBlock2,
    //         TetrisGravityComponent{
    //             .gravity = 40.f
    //         });
    // gCoordinator.AddComponent(
    //         mTestBlock2,
    //         BoxColliderComponent{
    //             .position = Vector2(960.f, 140.f),
    //             .w = 40,
    //             .h = 40
    //         });
    // gCoordinator.AddComponent(
    //         mTestBlock2,
    //         MoveComponent{
    //             .distance = 40.f
    //         });
    //
    // mTestBlock3 = gCoordinator.CreateEntity();
    // gCoordinator.AddComponent(
    //         mTestBlock3,
    //         TextureComponent{
    //             .texture = nullptr,
    //             .path = "src/Assets/yellowBlock.png",
    //         });
    // gCoordinator.AddComponent(
    //         mTestBlock3,
    //         TransformComponent{
    //             .position = Vector2(920.f, 180.f),
    //             .rotation = Vector2(),
    //             .scale = Vector2()
    //         });
    // gCoordinator.AddComponent(
    //         mTestBlock3,
    //         TetrisGravityComponent{
    //             .gravity = 40.f
    //         });
    // gCoordinator.AddComponent(
    //         mTestBlock3,
    //         BoxColliderComponent{
    //             .position = Vector2(920.f, 180.f),
    //             .w = 40,
    //             .h = 40
    //         });
    // gCoordinator.AddComponent(
    //         mTestBlock3,
    //         MoveComponent{
    //             .distance = 40.f
    //         });
    //
    // mTestBlock4 = gCoordinator.CreateEntity();
    // gCoordinator.AddComponent(
    //         mTestBlock4,
    //         TextureComponent{
    //             .texture = nullptr,
    //             .path = "src/Assets/yellowBlock.png",
    //         });
    // gCoordinator.AddComponent(
    //         mTestBlock4,
    //         TransformComponent{
    //             .position = Vector2(960.f, 220.f),
    //             .rotation = Vector2(),
    //             .scale = Vector2()
    //         });
    // gCoordinator.AddComponent(
    //         mTestBlock4,
    //         TetrisGravityComponent{
    //             .gravity = 40.f
    //         });
    // gCoordinator.AddComponent(
    //         mTestBlock4,
    //         BoxColliderComponent{
    //             .position = Vector2(960.f, 220.f),
    //             .w = 40,
    //             .h = 40
    //         });
    // gCoordinator.AddComponent(
    //         mTestBlock4,
    //         MoveComponent{
    //             .distance = 40.f
    //         });

}

void PlayState::Exit()
{
    // Destroy the Texture component
    auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(mBackground);
    textureComponent.destroy();

    gCoordinator.DestroyEntity(mBackground);
}
