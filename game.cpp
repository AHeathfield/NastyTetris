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

#include "src/Systems/RenderSystem.h"
#include "src/Systems/MouseButtonSystem.h"
#include "src/Systems/PhysicsSystem.h"
#include "src/Systems/CollisionSystem.h"
#include "src/Systems/PlayerEventSystem.h"
#include "src/Systems/ShapeSystem.h"

// States
#include "src/States/State.h"
#include "src/States/TitleState.h"
#include "src/States/PlayState.h"

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
    TTF_CloseFont( gFont );
    gFont = nullptr;

    //Quit SDL subsystems
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //Calculate the sides of rect A
    int aMinX{ a.x };
    int aMaxX{ a.x + a.w };
    int aMinY{ a.y };
    int aMaxY{ a.y + a.h };

    //Calculate the sides of rect B
    int bMinX{ b.x };
    int bMaxX{ b.x + b.w };
    int bMinY{ b.y };
    int bMaxY{ b.y + b.h };

    //If left side of A is the the right of B
    if( aMinX >= bMaxX )
    {
        return false;
    }

    //If the right side of A to the left of B
    if( aMaxX <= bMinX )
    {
        return false;
    }

    //If the top side of A is below B
    if( aMinY >= bMaxY )
    {
        return false;
    }

    //If the bottom side of A is above B
    if( aMaxY <= bMinY )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
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

    // Shape System
    auto shapeSystem = gCoordinator.RegisterSystem<ShapeSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<MoveComponent>());
        gCoordinator.SetSystemSignature<ShapeSystem>(signature);
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
        if (renderSystem->LoadMedia() == false)
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
                // renderSystem->LoadMedia();
                // prevState = gCurrentState;
                prevState->Exit();
                delete prevState;
                gCurrentState->Enter();
                renderSystem->LoadMedia();
                prevState = gCurrentState;
            }

            // This loads the new Shape
            if (typeid(*gCurrentState) == typeid(PlayState))
            {
                bool isNewShape = shapeSystem->Update();
                if (isNewShape)
                {
                    renderSystem->LoadMedia();
                }
            }

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
                playerEventSystem->HandleEvent(e, shapeSystem->currentShape);
            }
            
            if (updateTimer.getTimeS() > deltaTime)
            {
                // Physics will move the colliders, collision will check if there are collisions if so move back to where they were, render draws everything
                physicsSystem->Update();
                updateTimer.reset();
            }

            collisionSystem->UpdateCollisions(shapeSystem->currentShape);
            collisionSystem->UpdateTransforms();
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
