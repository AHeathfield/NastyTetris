#include "PlayState.h"

extern Coordinator gCoordinator;

void PlayState::Enter()
{
    mBackground = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBackground,
            TextureComponent{
                .texture = nullptr,
                .path = "src/Assets/testBG.png",
                .width = 1920,
                .height = 1080
            });

    gCoordinator.AddComponent(
            mBackground,
            TransformComponent{
                .position = Vector2(500.f, 200.f),
                .rotation = Vector2(),
                .scale = Vector2()
            });
}

void PlayState::Exit()
{
    // Destroy the Texture component
    auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(mBackground);
    textureComponent.destroy();

    gCoordinator.DestroyEntity(mBackground);
}
