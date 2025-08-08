// External Libs
#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <sstream>
#include <typeinfo>

// ECS
#include "src/Core/ECS.h"
#include "src/Components/TextureComponent.h"
#include "src/Components/TransformComponent.h"
#include "src/Components/BoxColliderComponent.h"
#include "src/Components/TetrisGravityComponent.h"
#include "src/Components/MoveComponent.h"
#include "src/Components/BoundaryComponent.h"
#include "src/Components/HoldComponent.h"
#include "src/Components/RowComponent.h"
#include "src/Components/ScoreComponent.h"

#include "src/Systems/RenderSystem.h"
#include "src/Systems/MouseButtonSystem.h"
#include "src/Systems/PhysicsSystem.h"
#include "src/Systems/CollisionSystem.h"
#include "src/Systems/PlayerEventSystem.h"
#include "src/Systems/ShapeSystem.h"
#include "src/Systems/PlayShapeSystem.h"
#include "src/Systems/HoldSystem.h"
#include "src/Systems/RowSystem.h"
#include "src/Systems/ScoreSystem.h"
#include "src/Systems/TextEventSystem.h"

// States
#include "src/States/State.h"
#include "src/States/TitleState.h"
#include "src/States/PlayState.h"
#include "src/States/ScoreState.h"

// Core
#include "src/Core/Timer.h"


Coordinator gCoordinator;
State* gCurrentState;

/* Constants */
constexpr int kScreenFps{ 60 };

//Channel constants
enum class eEffectChannel
{
    Scratch = 0,
    High = 1,
    Medium = 2,
    Low = 3,
    Total = 4
};


/* Function Prototypes */
//Starts up SDL and creates window
bool init();

//Loads media handled by RenderSystem
// bool loadMedia();

//Frees media and shuts down SDL
void close();

//Check collision between two AABBs
bool checkCollision(SDL_Rect a, SDL_Rect b);


/* Global Variables */
//Global font
TTF_Font* gFont{ nullptr };

//Playback audio device
SDL_AudioDeviceID gAudioDeviceId{ 0 };

//Allocated channel count
int gChannelCount{ 0 };

//The music that will be played
Mix_Music* gMusic{ nullptr };

//The sound effects that will be used
Mix_Chunk* gScratch{ nullptr };
Mix_Chunk* gHigh{ nullptr };
Mix_Chunk* gMedium{ nullptr };
Mix_Chunk* gLow{ nullptr };



/* Function Implementations */
bool init()
{
    //Initialization flag
    bool success{ true };

    //Initialize SDL with audio
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == false )
    {
        SDL_Log( "SDL could not initialize! SDL error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Initialize the font loading
        if( TTF_Init() == false )
        {
            SDL_Log( "SDL_ttf could not initialize! SDL_ttf error: %s\n", SDL_GetError() );
            success = false;
        }

        //Set audio spec
        SDL_AudioSpec audioSpec;
        SDL_zero( audioSpec );
        audioSpec.format = SDL_AUDIO_F32;
        audioSpec.channels = 2;
        audioSpec.freq = 44100;

        //Open audio device
        gAudioDeviceId = SDL_OpenAudioDevice( SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec );
        if( gAudioDeviceId == 0 )
        {
            SDL_Log( "Unable to open audio! SDL error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Initialize SDL_mixer
            if( Mix_OpenAudio( gAudioDeviceId, nullptr ) == false )
            {
                SDL_Log( "SDL_mixer could not initialize! SDL_mixer error: %s\n", SDL_GetError() );
                success = false;
            }
        }
    }

    return success;
}


void close()
{
    //Free music
    Mix_FreeMusic( gMusic );
    gMusic = nullptr;

    //Free sound effects
    Mix_FreeChunk( gScratch );
    gScratch =  nullptr;
    Mix_FreeChunk( gHigh );
    gHigh =  nullptr;
    Mix_FreeChunk( gMedium );
    gMedium =  nullptr;
    Mix_FreeChunk( gLow );
    gLow =  nullptr;

    //Close mixer audio
    Mix_CloseAudio();

    //Close audio device
    SDL_CloseAudioDevice( gAudioDeviceId );
    gAudioDeviceId = 0;

    //Free font
    // TTF_CloseFont( gFont );
    // gFont = nullptr;

    //Quit SDL subsystems
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}


int main( int argc, char* args[] )
{
    // Final exit code
    int exitCode{ 0 };

    // Timer for when we want to run updates
    Timer updateTimer = Timer();
    float deltaTime = 0.8f;

    // Timer to cap frame rate
    Timer capTimer = Timer();
    Uint64 renderingNS = 0; // Time spent rendering

    // Setting Initial State and next State
    gCurrentState = new TitleState();
    State* prevState = gCurrentState;

    // Initalizing ECS Stuff
    gCoordinator.Init();
    bool successInit = init();

    // Registering Components
    gCoordinator.RegisterComponent<TextureComponent>();
    gCoordinator.RegisterComponent<TransformComponent>();
    gCoordinator.RegisterComponent<TetrisGravityComponent>();
    gCoordinator.RegisterComponent<BoxColliderComponent>();
    gCoordinator.RegisterComponent<MoveComponent>();
    gCoordinator.RegisterComponent<BoundaryComponent>();
    gCoordinator.RegisterComponent<HoldComponent>();
    gCoordinator.RegisterComponent<RowComponent>();
    gCoordinator.RegisterComponent<ScoreComponent>();
    gCoordinator.RegisterComponent<ButtonComponent*>();

    // Registering Systems
    // Render System
    auto renderSystem = gCoordinator.RegisterSystem<RenderSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<TextureComponent>());
        signature.set(gCoordinator.GetComponentType<TransformComponent>());
        // TODO: EXPLAIN I FORGOT THIS LINE ON NEXT STREAM
        gCoordinator.SetSystemSignature<RenderSystem>(signature);
    }
    renderSystem->Init();

    // Mouse Button System
    auto mouseButtonSystem = gCoordinator.RegisterSystem<MouseButtonSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<TextureComponent>());
        signature.set(gCoordinator.GetComponentType<TransformComponent>());
        signature.set(gCoordinator.GetComponentType<ButtonComponent*>());
        gCoordinator.SetSystemSignature<MouseButtonSystem>(signature);
    }

    // Physics System
    auto physicsSystem = gCoordinator.RegisterSystem<PhysicsSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<BoxColliderComponent>());
        signature.set(gCoordinator.GetComponentType<TetrisGravityComponent>());
        gCoordinator.SetSystemSignature<PhysicsSystem>(signature);
    }

    // Collision System
    auto collisionSystem = gCoordinator.RegisterSystem<CollisionSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<TransformComponent>());
        signature.set(gCoordinator.GetComponentType<BoxColliderComponent>());
        gCoordinator.SetSystemSignature<CollisionSystem>(signature);
    }

    // Player Event System
    auto playerEventSystem = gCoordinator.RegisterSystem<PlayerEventSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<MoveComponent>());
        signature.set(gCoordinator.GetComponentType<BoxColliderComponent>());
        gCoordinator.SetSystemSignature<PlayerEventSystem>(signature);
    }

    // Text Event System (SHOULD NOT BE A SYSTEM SHOULD BE HANDLED BY STATE)
    auto textEventSystem = gCoordinator.RegisterSystem<TextEventSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<TransformComponent>());
        signature.set(gCoordinator.GetComponentType<TextureComponent>());
        gCoordinator.SetSystemSignature<TextEventSystem>(signature);
    }

    // Shape System
    auto shapeSystem = gCoordinator.RegisterSystem<ShapeSystem>();
    {
        Signature signature;
        // signature.set(gCoordinator.GetComponentType<MoveComponent>());
        gCoordinator.SetSystemSignature<ShapeSystem>(signature);
    }

    // Play Shape System
    auto playShapeSystem = gCoordinator.RegisterSystem<PlayShapeSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<MoveComponent>());
        gCoordinator.SetSystemSignature<PlayShapeSystem>(signature);
    }

    // Hold System
    auto holdSystem = gCoordinator.RegisterSystem<HoldSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<HoldComponent>());
        gCoordinator.SetSystemSignature<HoldSystem>(signature);
    }

    // Row System
    auto rowSystem = gCoordinator.RegisterSystem<RowSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<RowComponent>());
        signature.set(gCoordinator.GetComponentType<TransformComponent>());
        signature.set(gCoordinator.GetComponentType<TextureComponent>());
        gCoordinator.SetSystemSignature<RowSystem>(signature);
    }

    // Score System
    auto scoreSystem = gCoordinator.RegisterSystem<ScoreSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<ScoreComponent>());
        signature.set(gCoordinator.GetComponentType<TransformComponent>());
        signature.set(gCoordinator.GetComponentType<TextureComponent>());
        gCoordinator.SetSystemSignature<ScoreSystem>(signature);
    }

    // Other systems...


    // TODO: Currently RenderSystem intializes window and renderer
    //Initialize
    if( successInit == false )
    {
        SDL_Log( "Unable to initialize program!\n" );
        exitCode = 1;
    }
    else
    {
        //The quit flag
        bool quit{ false };
        bool stateChanged = false;
        int moveCount = 0;
        bool canMove = true;

        //The event data
        SDL_Event e;
        SDL_zero( e );

        // SETTING ENTITIES
        // When you do AddComponent, it automatically updates which systems should have it
        // State->Enter() calls gCoordinator.AddComponent
        gCurrentState->Enter();

        // DO THIS AFTER LOADING TEXTURES!!!
        if (renderSystem->LoadAllMedia() == false)
        {
            SDL_Log( "Unable to load media!\n" );
            exitCode = 2;
        }


        updateTimer.start();

        // =================== Main Loop ========================
        while( quit == false )
        {
            //Start frame time
            capTimer.start();

            // Setting new state
            if (gCurrentState != prevState)
            {
                // renderSystem->LoadAllMedia();
                // prevState = gCurrentState;
                prevState->Exit();
                delete prevState;
                gCurrentState->Enter();
                renderSystem->LoadAllMedia();
                prevState = gCurrentState;
            }

            // This handles the next shapes and the playable shape
            // if (typeid(*gCurrentState) == typeid(PlayState))
            // {
            //     bool isNewShape = shapeSystem->Update();
            //     if (isNewShape)
            //     {
            //         renderSystem->LoadAllMedia();
            //     }
            //
            //     // Loads the playing shape when needed
            //     if (playShapeSystem->isNoPlayingShape())
            //     {
            //         playShapeSystem->PlayNextShape(shapeSystem->GetNextShape());
            //         playShapeSystem->UpdateRefEntity();
            //     }
            // }

            //Get event data
            while( SDL_PollEvent( &e ) == true )
            {
                //If event is quit type
                if( e.type == SDL_EVENT_QUIT )
                {
                    //End the main loop
                    quit = true;
                }

                mouseButtonSystem->HandleEvent(&e);
                playerEventSystem->HandleEvent(e);

                if (typeid(*gCurrentState) == typeid(ScoreState))
                {
                    textEventSystem->HandleEvent(e);
                }
            }
            

            if (updateTimer.getTimeS() > deltaTime)
            {
                // This handles the next shapes and the playable shape
                if (typeid(*gCurrentState) == typeid(PlayState))
                {
                    bool isNewShape = shapeSystem->Update();
                    if (isNewShape)
                    {
                        renderSystem->LoadAllMedia();
                    }

                    // Loads the playing shape when needed
                    if (playShapeSystem->isNoPlayingShape())
                    {
                        playShapeSystem->PlayNextShape(shapeSystem->GetNextShape());
                        playShapeSystem->UpdateRefEntity();
                    }
                }

                // Physics will move the colliders, collision will check if there are collisions if so move back to where they were, render draws everything
                physicsSystem->Update();

                // Deletes the rows that are completed
                rowSystem->DeleteRows();

                // Updates the delta time
                if (deltaTime > 0.3f)
                {
                    deltaTime = 0.8f - (scoreSystem->level - 1) * 0.05f;
                }

                updateTimer.reset();
            }

            collisionSystem->UpdateCollisions(playShapeSystem->currentShape);
            collisionSystem->UpdateTransforms();
            holdSystem->Update();
            playShapeSystem->Update();
            rowSystem->Update();
            // scoreSystem->Update();
            renderSystem->Update();

            // If time remaining in frame
            constexpr Uint64 nsPerFrame = 1000000000 / kScreenFps; 
            Uint64 frameNs = capTimer.getTicksNS();
            if ( frameNs < nsPerFrame)
            {
                SDL_DelayNS(nsPerFrame - frameNs);
            }
        }
        // =======================================================
        gCurrentState->Exit();
    }

    //Clean up
    delete gCurrentState;
    gCurrentState = nullptr;
    prevState = nullptr;

    renderSystem->Close();
    close();

    return exitCode;
}
