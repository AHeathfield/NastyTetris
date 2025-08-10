#include "ScoreState.h"
#include "PlayState.h"

extern Coordinator gCoordinator;

void ScoreState::Enter()
{
    mBackground = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBackground,
            TextureComponent{
                .texture = nullptr,
                .path = "src/Assets/HighScoreBG.png",
            });

    gCoordinator.AddComponent(
            mBackground,
            TransformComponent{
                .position = Vector2(),
                .rotation = Vector2(),
                .scale = Vector2()
            });


    mPlayButton = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mPlayButton,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 517.f, 183.f},
                .path = "src/Assets/playButtonSpriteSheet.png",
            });
    gCoordinator.AddComponent(
            mPlayButton,
            TransformComponent{
                .position = Vector2(100.f, (1080 / 2.f) - (183.f / 2.f)),
                .rotation = Vector2(),
                .scale = Vector2()
            });
    ButtonComponent* button = new StateChangerButton(new PlayState());
    gCoordinator.AddComponent(
            mPlayButton,
            button
            );
    button = nullptr;


    // For text input
    auto renderSystem = gCoordinator.GetSystem<RenderSystem>();
    renderSystem->EnableTextInput();
    auto textEventSystem = gCoordinator.GetSystem<TextEventSystem>();
    textEventSystem->Init();

}

void ScoreState::Exit()
{
    auto renderSystem = gCoordinator.GetSystem<RenderSystem>();
    renderSystem->DisableTextInput();
    auto textEventSystem = gCoordinator.GetSystem<TextEventSystem>();
    textEventSystem->Close();

    // Destroy the Texture component
    auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(mBackground);
    textureComponent.destroy();
    textureComponent = gCoordinator.GetComponent<TextureComponent>(mPlayButton);
    textureComponent.destroy();

    gCoordinator.DestroyEntity(mBackground);
    gCoordinator.DestroyEntity(mPlayButton);
}
