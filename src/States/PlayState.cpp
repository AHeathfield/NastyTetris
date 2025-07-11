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

    mBottomCollider = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBottomCollider,
            BoxColliderComponent{
                .position = Vector2(720.f, 940.f),
                .w = static_cast<float>(1200 - 720),
                .h = 40.f
            });
    gCoordinator.AddComponent(
            mBottomCollider,
            TransformComponent{
                .position = Vector2(720.f, 940.f),
            });
}

void PlayState::Exit()
{
    // Destroy the Texture component
    auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(mBackground);
    textureComponent.destroy();

    gCoordinator.DestroyEntity(mBackground);
}
