#include "PlayState.h"
#include <unordered_map>
#include <vector>

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
    
    mTopBoundCollider = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mTopBoundCollider,
            BoxColliderComponent{
                .position = Vector2(720.f, 100.f),
                .w = 1200 - 720,
                .h = 40
            });
    gCoordinator.AddComponent(
            mTopBoundCollider,
            TransformComponent{
                .position = Vector2(720.f, 100.f),
            });
    gCoordinator.AddComponent(
            mTopBoundCollider,
            BoundaryComponent{
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


    mBGMusic = gCoordinator.CreateEntity();
    std::unordered_map<std::string, Audio> entityAudios;
    Audio bgMusic = {
        .filePath = "src/Assets/Music/NastyTetrisBGMusic.wav",
        .isMusic = true,
        .isPlaying = true
    };
    entityAudios.insert({"Music", bgMusic});
    gCoordinator.AddComponent(
            mBGMusic,
            AudioComponent{
                .audios = entityAudios
            });
    auto audioSystem = gCoordinator.GetSystem<AudioSystem>();
    audioSystem->LoadAudio(bgMusic);

    // Initializing Score
    auto scoreSystem = gCoordinator.GetSystem<ScoreSystem>();
    scoreSystem->Init();

    // Test Text
    // Entity testText = gCoordinator.CreateEntity();
    // gCoordinator.AddComponent(
    //         testText,
    //         TextureComponent{
    //             .texture = nullptr,
    //             .path = "src/Assets/8bit16.ttf",
    //             .isText = true,
    //             .fontSize = 40,
    //             .text = "00000000",
    //         });
    // gCoordinator.AddComponent(
    //         testText, 
    //         TransformComponent{
    //             .position = Vector2(360.f, 600.f)
    //         });
}

void PlayState::Exit()
{
    // SDL_Log("EXITING PLAY");

    // Stopping the music
    auto& audioComponent = gCoordinator.GetComponent<AudioComponent>(mBGMusic);
    audioComponent.Stop("Music");
    gCoordinator.DestroyEntity(mBGMusic);

    // Destroy the Texture component
    auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(mBackground);
    textureComponent.destroy();

    gCoordinator.DestroyEntity(mBackground);

    // Destroying colliders
    std::vector<Entity> entities = {mBottomBoundCollider, mRightBoundCollider, mLeftBoundCollider, mTopBoundCollider};
    for (const auto& entity : entities)
    {
        gCoordinator.DestroyEntity(entity);
    }

    // Closing Systems involved
    gCoordinator.GetSystem<ScoreSystem>()->Close();
    gCoordinator.GetSystem<PlayShapeSystem>()->Close();
    gCoordinator.GetSystem<HoldSystem>()->Close();
    gCoordinator.GetSystem<ShapeSystem>()->Close();
    gCoordinator.GetSystem<RowSystem>()->Close();
}
