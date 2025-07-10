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
                .position = Vector2(920.f, 142.f),
                .rotation = Vector2(),
                .scale = Vector2()
            });
    gCoordinator.AddComponent(
            mTestBlock,
            TetrisGravityComponent{
                .gravity = 40.f
            });
}

void PlayState::Exit()
{
    // Destroy the Texture component
    auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(mBackground);
    textureComponent.destroy();

    gCoordinator.DestroyEntity(mBackground);
}
