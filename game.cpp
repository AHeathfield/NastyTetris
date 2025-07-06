/* Headers */
//Using SDL, SDL_image, SDL_ttf SDL_mixer, and STL string
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <sstream>

/* Constants */
//Screen dimension constants
constexpr int kScreenWidth{ 640 };
constexpr int kScreenHeight{ 480 };
constexpr int kScreenFps{ 60 };

//Level dimensions
constexpr int kLevelWidth{ 1280 };
constexpr int kLevelHeight{ 960 };

//Text rendering constants
const char* kStartingText{ "Some Text" };
const SDL_Color kTextColor{ 0x00, 0x00, 0x00, 0xFF };

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

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Check collision between two AABBs
bool checkCollision(SDL_Rect a, SDL_Rect b);

class Square
{
    public:
        //The dimensions of the square
        static constexpr int kSquareWidth = 20;
        static constexpr int kSquareHeight = 20;

        //Maximum axis velocity of the square
        static constexpr int kSquareVel = 10;

        //Initializes the variables
        Square();

        //Takes key presses and adjusts the square's velocity
        void handleEvent( SDL_Event& e );

        //Moves the square
        void move( SDL_Rect collider );

        //Shows the square on the screen
        void render();

    private:
        //The collision box
        SDL_Rect mCollisionBox;

        //The velocity of the square
        int mVelX, mVelY;
};

class Dot
{
    public:
        //The dimensions of the dot
        static constexpr int kDotWidth = 20;
        static constexpr int kDotHeight = 20;

        //Maximum axis velocity of the dot
        static constexpr int kDotVel = 10;

        //Initializes the variables
        Dot();

        //Takes key presses and adjusts the dot's velocity
        void handleEvent( SDL_Event& e );

        //Moves the dot
        void move();

        //Shows the dot on the screen
        void render( SDL_FRect camera );

        //Position accessors
        int getPosX();
        int getPosY();

    private:
        //The X and Y offsets of the dot
        int mPosX, mPosY;

        //The velocity of the dot
        int mVelX, mVelY;
};

class LTimer
{
    public:
        //Initializes variables
        LTimer();

        //The various clock actions
        void start();
        void stop();
        void pause();
        void unpause();

        //Gets the timer's time
        Uint64 getTicksNS();

        //Checks the status of the timer
        bool isStarted();
        bool isPaused();

    private:
        //The clock time when the timer started
        Uint64 mStartTicks;

        //The ticks stored when the timer was paused
        Uint64 mPausedTicks;

        //The timer status
        bool mPaused;
        bool mStarted;
};


class LButton
{
    public:
        //Button dimensions
        static constexpr int kButtonWidth = 300;
        static constexpr int kButtonHeight = 200;

        //Initializes internal variables
        LButton();

        //Sets top left position
        void setPosition( float x, float y );

        //Handles mouse event
        void handleEvent( SDL_Event* e );
    
        //Shows button sprite
        void render();

    private:
        enum class eButtonSprite
        {
            MouseOut = 0,
            MouseOverMotion = 1,
            MouseDown = 2,
            MouseUp = 3
        };

        //Top left position
        SDL_FPoint mPosition;

        //Currently used global sprite
        eButtonSprite mCurrentSprite;
};




/* Class Prototypes */
class LTexture
{
public:
    // Symbolic constant
    static constexpr float kOriginalSize = -1.f;
    
    //Initializes texture variables
    LTexture();

    //Cleans up texture variables
    ~LTexture();

    //Loads texture from disk
    bool loadFromFile( std::string path );

    #if defined(SDL_TTF_MAJOR_VERSION)
    //Creates texture from text
    bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
    #endif

    //Cleans up texture
    void destroy();

     //Sets color modulation
    void setColor( Uint8 r, Uint8 g, Uint8 b);

    //Sets opacity
    void setAlpha( Uint8 alpha );

    //Sets blend mode
    void setBlending( SDL_BlendMode blendMode );

    //Draws texture
    void render( float x, float y, SDL_FRect* clip = nullptr, float width = kOriginalSize, float height = kOriginalSize, double degrees = 0.0, SDL_FPoint* center = nullptr, SDL_FlipMode flipMode = SDL_FLIP_NONE );

    //Gets texture attributes
    int getWidth();
    int getHeight();
    bool isLoaded();

private:
    //Contains texture data
    SDL_Texture* mTexture;

    //Texture dimensions
    int mWidth;
    int mHeight;
};



/* Global Variables */
//The window we'll be rendering to
SDL_Window* gWindow{ nullptr };

//The renderer used to draw to the window
SDL_Renderer* gRenderer{ nullptr };

//Global font
TTF_Font* gFont{ nullptr };

// Textures
LTexture gDotTexture, gBgTexture;

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



/* Class Implementations */
//Square implementation
Square::Square():
    mCollisionBox{ 0, 0, kSquareWidth, kSquareHeight },
    mVelX{ 0 },
    mVelY{ 0 }
{

}

void Square::handleEvent( SDL_Event& e )
{
    //If a key was pressed
    if( e.type == SDL_EVENT_KEY_DOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.key )
        {
            case SDLK_UP: mVelY -= kSquareVel; break;
            case SDLK_DOWN: mVelY += kSquareVel; break;
            case SDLK_LEFT: mVelX -= kSquareVel; break;
            case SDLK_RIGHT: mVelX += kSquareVel; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_EVENT_KEY_UP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.key )
        {
            case SDLK_UP: mVelY += kSquareVel; break;
            case SDLK_DOWN: mVelY -= kSquareVel; break;
            case SDLK_LEFT: mVelX += kSquareVel; break;
            case SDLK_RIGHT: mVelX -= kSquareVel; break;
        }
    }
}

void Square::move( SDL_Rect collider )
{
    //Move the square left or right
    mCollisionBox.x += mVelX;

    //If the square went off screen or hit the wall
    if( ( mCollisionBox.x < 0 ) || ( mCollisionBox.x + kSquareWidth > kScreenWidth ) || checkCollision( mCollisionBox, collider ) )
    {
        //Move back
        mCollisionBox.x -= mVelX;
    }

    //Move the square up or down
    mCollisionBox.y += mVelY;

    //If the square went off screen or hit the wall
    if( ( mCollisionBox.y < 0 ) || ( mCollisionBox.y + kSquareHeight > kScreenHeight ) || checkCollision( mCollisionBox, collider ) )
    {
        //Move back
        mCollisionBox.y -= mVelY;
    }
}

void Square::render()
{
    //Show the square
    SDL_FRect drawingRect{ static_cast<float>( mCollisionBox.x ), static_cast<float>( mCollisionBox.y ), static_cast<float>( mCollisionBox.w ), static_cast<float>( mCollisionBox.h ) };
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );   
    SDL_RenderRect( gRenderer, &drawingRect );
}


//Dot Implementation
Dot::Dot():
    mPosX{ 0 },
    mPosY{ 0 },
    mVelX{ 0 },
    mVelY{ 0 }
{

}

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
    if( e.type == SDL_EVENT_KEY_DOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.key )
        {
            case SDLK_UP: mVelY -= kDotVel; break;
            case SDLK_DOWN: mVelY += kDotVel; break;
            case SDLK_LEFT: mVelX -= kDotVel; break;
            case SDLK_RIGHT: mVelX += kDotVel; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_EVENT_KEY_UP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.key )
        {
            case SDLK_UP: mVelY += kDotVel; break;
            case SDLK_DOWN: mVelY -= kDotVel; break;
            case SDLK_LEFT: mVelX += kDotVel; break;
            case SDLK_RIGHT: mVelX -= kDotVel; break;
        }
    }
}

void Dot::move()
{
    //Move the dot left or right
    mPosX += mVelX;

    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + kDotWidth > kLevelWidth ) )
    {
        //Move back
        mPosX -= mVelX;
    }

    //Move the dot up or down
    mPosY += mVelY;

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + kDotHeight > kLevelHeight ) )
    {
        //Move back
        mPosY -= mVelY;
    }
}

void Dot::render( SDL_FRect camera )
{
    //Show the dot
    gDotTexture.render( static_cast<float>( mPosX ) - camera.x, static_cast<float>( mPosY ) - camera.y );
}

int Dot::getPosX()
{
    return mPosX;
}

int Dot::getPosY()
{
    return mPosY;
}


//LTimer Implementation
LTimer::LTimer():
    mStartTicks{ 0 },
    mPausedTicks{ 0 },

    mPaused{ false },
    mStarted{ false }
{

}

void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicksNS();
    mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicksNS() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicksNS() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint64 LTimer::getTicksNS()
{
    //The actual timer time
    Uint64 time{ 0 };

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicksNS() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isPaused() 
{
    return mPaused;
}

bool LTimer::isStarted()
{
    return mStarted;
}

//LButton Implementation
LButton::LButton():
    mPosition{ 0.f, 0.f },
    mCurrentSprite{ eButtonSprite::MouseOut }
{

}

void LButton::setPosition( float x, float y )
{
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::handleEvent( SDL_Event* e )
{
    //If mouse event happened
    if( e->type == SDL_EVENT_MOUSE_MOTION || e->type == SDL_EVENT_MOUSE_BUTTON_DOWN || e->type == SDL_EVENT_MOUSE_BUTTON_UP )
    {
        //Get mouse position
        float x = -1.f, y = -1.f;
        SDL_GetMouseState( &x, &y );

        //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if( x < mPosition.x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > mPosition.x + kButtonWidth )
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < mPosition.y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > mPosition.y + kButtonHeight )
        {
            inside = false;
        }


        //Mouse is outside button
        if( !inside )
        {
            mCurrentSprite = eButtonSprite::MouseOut;
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_EVENT_MOUSE_MOTION:
                    mCurrentSprite = eButtonSprite::MouseOverMotion;
                    break;
            
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    mCurrentSprite = eButtonSprite::MouseDown;
                    break;
                
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    mCurrentSprite = eButtonSprite::MouseUp;
                    break;
            }
        }
    }
}

void LButton::render()
{
    //Define sprites
    SDL_FRect spriteClips[] = {
        { 0.f, 0 * kButtonHeight, kButtonWidth, kButtonHeight },
        { 0.f, 1 * kButtonHeight, kButtonWidth, kButtonHeight },
        { 0.f, 2 * kButtonHeight, kButtonWidth, kButtonHeight },
        { 0.f, 3 * kButtonHeight, kButtonWidth, kButtonHeight },
    };


    //Show current button sprite
    // gButtonSpriteTexture.render( mPosition.x, mPosition.y, &spriteClips[ static_cast<int>( mCurrentSprite ) ] );
}


//LTexture Implementation
LTexture::LTexture():
    //Initialize texture variables
    mTexture{ nullptr },
    mWidth{ 0 },
    mHeight{ 0 }
{

}

LTexture::~LTexture()
{
    //Clean up texture
    destroy();
}

bool LTexture::loadFromFile( std::string path )
{
    //Clean up texture if it already exists
    destroy();

    //Load surface
    if( SDL_Surface* loadedSurface = IMG_Load( path.c_str() ); loadedSurface == nullptr )
    {
        SDL_Log( "Unable to load image %s! SDL_image error: %s\n", path.c_str(), SDL_GetError() );
    }
    else
    {
        //Color key image
        if( SDL_SetSurfaceColorKey( loadedSurface, true, SDL_MapSurfaceRGB( loadedSurface, 0x00, 0xFF, 0xFF ) ) == false )
        {
            SDL_Log( "Unable to color key! SDL error: %s", SDL_GetError() );
        }
        else
        {
            //Create texture from surface
            if( mTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface ); mTexture == nullptr )
            {
                SDL_Log( "Unable to create texture from loaded pixels! SDL error: %s\n", SDL_GetError() );
            }
            else
            {
                //Get image dimensions
                mWidth = loadedSurface->w;
                mHeight = loadedSurface->h;
            }
        }
        
        //Clean up loaded surface
        SDL_DestroySurface( loadedSurface );
    }

    //Return success if texture loaded
    return mTexture != nullptr;
}


int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

bool LTexture::isLoaded()
{
    return mTexture != nullptr;
}

void LTexture::destroy()
{
    //Clean up texture
    SDL_DestroyTexture( mTexture );
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

void LTexture::render( float x, float y, SDL_FRect* clip, float width, float height, double degrees, SDL_FPoint* center, SDL_FlipMode flipMode )
{
    //Set texture position
    SDL_FRect dstRect{ x, y, static_cast<float>( mWidth ), static_cast<float>( mHeight ) };

    //Default to clip dimensions if clip is given
    if( clip != nullptr )
    {
        dstRect.w = clip->w;
        dstRect.h = clip->h;
    }

    //Resize if new dimensions are given
    if( width > 0 )
    {
        dstRect.w = width;
    }
    if( height > 0 )
    {
        dstRect.h = height;
    }

    //Render texture
    SDL_RenderTextureRotated( gRenderer, mTexture, clip, &dstRect, degrees, center, flipMode );
}

void LTexture::setColor( Uint8 r, Uint8 g, Uint8 b )
{
    SDL_SetTextureColorMod( mTexture, r, g, b );
}

void LTexture::setAlpha( Uint8 alpha )
{
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::setBlending( SDL_BlendMode blendMode )
{
    SDL_SetTextureBlendMode( mTexture, blendMode );
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    //Clean up existing texture
    destroy();

    //Load text surface
    if( SDL_Surface* textSurface = TTF_RenderText_Blended( gFont, textureText.c_str(), 0, textColor ); textSurface == nullptr )
    {
        SDL_Log( "Unable to render text surface! SDL_ttf Error: %s\n", SDL_GetError() );
    }
    else
    {
        //Create texture from surface
        if( mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface ); mTexture == nullptr )
        {
            SDL_Log( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Free temp surface
        SDL_DestroySurface( textSurface );
    }

    //Return success if texture loaded
    return mTexture != nullptr;
}
#endif


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
        //Create window with renderer
        if( SDL_CreateWindowAndRenderer( "SDL3 Tutorial: Textures and Extension Libraries", kScreenWidth, kScreenHeight, 0, &gWindow, &gRenderer ) == false )
        {
            SDL_Log( "Window could not be created! SDL error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Enable VSync
            if( SDL_SetRenderVSync( gRenderer, 1 ) == false )
            {
                SDL_Log( "Could not enable VSync! SDL error: %s\n", SDL_GetError() );
                success = false;
            }
            
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
    }

    return success;
}


bool loadMedia()
{
    //File loading flag
    bool success{ true };

    
    //Load scene font
    // std::string fontPath{ "17-text-input-and-clipboard-handling/lazy.ttf" };
    // if( gFont = TTF_OpenFont( fontPath.c_str(), 28 ); gFont == nullptr )
    // {
    //     SDL_Log( "Could not load %s! SDL_ttf Error: %s\n", fontPath.c_str(), SDL_GetError() );
    //     success = false;
    // }
    // else
    // {
    //     //Load text
    //     SDL_Color textColor{ 0x00, 0x00, 0x00, 0xFF };
    //     if( gPromptTextTexture.loadFromRenderedText( "Enter Text:", textColor ) == false )
    //     {
    //         SDL_Log( "Could not load text texture %s! SDL_ttf Error: %s\n", fontPath.c_str(), SDL_GetError() );
    //         success = false;
    //     }
    //
    //     if( gInputTextTexture.loadFromRenderedText( kStartingText, textColor ) == false )
    //     {
    //         SDL_Log( "Could not load text texture %s! SDL_ttf Error: %s\n", fontPath.c_str(), SDL_GetError() );
    //         success = false;
    //     }
    // }


    //Load scene images
    if( gDotTexture.loadFromFile( "src/Assets/dot.png" ) == false )
    {
        SDL_Log( "Unable to load foo image!\n");
        success = false;
    }
    if( gBgTexture.loadFromFile( "src/Assets/testBG.png" ) == false )
    {
        SDL_Log( "Unable to load foo image!\n");
        success = false;
    }

    // //Load audio
    // if( gMusic = Mix_LoadMUS( "15-sound-effects-and-music/beat.wav" ); gMusic == nullptr )
    // {
    //     SDL_Log( "Unable to load music! SDL_mixer error: %s\n", SDL_GetError() );
    //     success = false;
    // }
    // if( gScratch = Mix_LoadWAV( "15-sound-effects-and-music/scratch.wav" ); gScratch == nullptr )
    // {
    //     SDL_Log( "Unable to load scratch sound! SDL_mixer error: %s\n", SDL_GetError() );
    //     success = false;
    // }
    // if( gHigh = Mix_LoadWAV( "15-sound-effects-and-music/high.wav" ); gHigh == nullptr )
    // {
    //     SDL_Log( "Unable to load high sound! SDL_mixer error: %s\n", SDL_GetError() );
    //     success = false;
    // }
    // if( gMedium = Mix_LoadWAV( "15-sound-effects-and-music/medium.wav" ); gMedium == nullptr )
    // {
    //     SDL_Log( "Unable to load medium sound! SDL_mixer error: %s\n", SDL_GetError() );
    //     success = false;
    // }
    // if( gLow = Mix_LoadWAV( "15-sound-effects-and-music/low.wav" ); gLow == nullptr )
    // {
    //     SDL_Log( "Unable to load low sound! SDL_mixer error: %s\n", SDL_GetError() );
    //     success = false;
    // }
    //
    // //Allocate channels
    // if( success )
    // {
    //     if( gChannelCount = Mix_AllocateChannels( static_cast<int>( eEffectChannel::Total ) ); gChannelCount != static_cast<int>( eEffectChannel::Total ) )
    //     {
    //         SDL_Log( "Unable to allocate channels! SDL_mixer error: %s\n", SDL_GetError() );
    //         success = false;
    //     }
    // }

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

    //Clean up texture
    gBgTexture.destroy();
    gDotTexture.destroy();

    //Free font
    TTF_CloseFont( gFont );
    gFont = nullptr;

    //Clean up button
    // gFpsTexture.destroy();

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    gRenderer = nullptr;
    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;

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
    //Final exit code
    int exitCode{ 0 };

    //Initialize
    if( init() == false )
    {
        SDL_Log( "Unable to initialize program!\n" );
        exitCode = 1;
    }
    else
    {
        //Load media
        if( loadMedia() == false )
        {
            SDL_Log( "Unable to load media!\n" );
            exitCode = 2;
        }
        else
        {
            //The quit flag
            bool quit{ false };

            //The event data
            SDL_Event e;
            SDL_zero( e );

             //VSync toggle
            bool vsyncEnabled{ true };

            //FPS cap toggle
            bool fpsCapEnabled{ false };

            //Timer to cap frame rate
            LTimer capTimer;

            // //Square we will be moving around on the screen
            // Square square;
            //
            // //The wall we will be colliding with
            // constexpr int kWallWidth = Square::kSquareWidth;
            // constexpr int kWallHeight = kScreenHeight - Square::kSquareHeight * 2;
            // SDL_Rect wall{ ( kScreenWidth - kWallWidth ) / 2, ( kScreenHeight - kWallHeight ) / 2, kWallWidth, kWallHeight };

            //Current animation frame
            int frame{ -1 };

            //Dot we will be moving around on the screen
            Dot dot;

            //Defines camera area
            SDL_FRect camera{ 0.f, 0.f, kScreenWidth, kScreenHeight };

            //Time spent rendering
            Uint64 renderingNS{ 0 };

            //In memory text stream
            std::stringstream timeText;

            //The current input text
            std::string inputText{ kStartingText };

            //Enable text input
            SDL_StartTextInput( gWindow );

            //Rotation degrees
            double degrees = 0.0;

            //Flipmode
            SDL_FlipMode flipMode = SDL_FLIP_NONE;

            // //Place buttons
            // constexpr int kButtonCount = 4;
            // LButton buttons[ kButtonCount ];
            // buttons[ 0 ].setPosition(                                    0,                                      0 );
            // buttons[ 1 ].setPosition( kScreenWidth - LButton::kButtonWidth,                                      0 );
            // buttons[ 2 ].setPosition(                                    0, kScreenHeight - LButton::kButtonHeight );
            // buttons[ 3 ].setPosition( kScreenWidth - LButton::kButtonWidth, kScreenHeight - LButton::kButtonHeight );
            //


            //The main loop
            while( quit == false )
            {
                // Start frame time
                capTimer.start();

                //The rerendering text flag
                bool renderText{ false };

                //Get event data
                while( SDL_PollEvent( &e ) == true )
                {
                    //If event is quit type
                    if( e.type == SDL_EVENT_QUIT )
                    {
                        //End the main loop
                        quit = true;
                    }

                    //Process dot events
                    dot.handleEvent( e );

                    //Special key input
                    // else if( e.type == SDL_EVENT_KEY_DOWN )
                    // {
                    //     //Handle backspace
                    //     if( e.key.key == SDLK_BACKSPACE && inputText.length() > 0 )
                    //     {
                    //         //lop off character
                    //         inputText.pop_back();
                    //         renderText = true;
                    //     }
                    //     //Handle copy
                    //     else if( e.key.key == SDLK_C && SDL_GetModState() & SDL_KMOD_CTRL )
                    //     {
                    //         SDL_SetClipboardText( inputText.c_str() );
                    //     }
                    //     //Handle paste
                    //     else if( e.key.key == SDLK_V && SDL_GetModState() & SDL_KMOD_CTRL )
                    //     {
                    //         //Copy text from temporary buffer
                    //         char* tempText{ SDL_GetClipboardText() };
                    //         inputText = tempText;
                    //         SDL_free( tempText );
                    //
                    //         renderText = true;
                    //     }
                    // }
                    //
                    // //Special text input event
                    // else if( e.type == SDL_EVENT_TEXT_INPUT )
                    // {
                    //     //If not copying or pasting
                    //     char firstChar{ static_cast<char>( toupper( e.text.text[ 0 ] ) ) };
                    //     if( !( SDL_GetModState() & SDL_KMOD_CTRL && ( firstChar == 'C' || firstChar == 'V' ) ) )
                    //     {
                    //         //Append character
                    //         inputText += e.text.text;
                    //         renderText = true;
                    //     }
                    // }

                    //Process square events
                    // square.handleEvent(e);


                    //Reset start time on return keypress
                    // else if( e.type == SDL_EVENT_KEY_DOWN )
                    // {
                    //      //VSync toggle
                    //     if( e.key.key == SDLK_RETURN )
                    //     {
                    //         vsyncEnabled = !vsyncEnabled;
                    //         SDL_SetRenderVSync( gRenderer, ( vsyncEnabled ) ? 1 : SDL_RENDERER_VSYNC_DISABLED );
                    //     }
                    //     //FPS cap toggle
                    //     else if( e.key.key == SDLK_SPACE )
                    //     {
                    //         fpsCapEnabled = !fpsCapEnabled;
                    //     }
                    // }


                    //Update screen

                    //On key press
                    // else if( e.type == SDL_EVENT_KEY_DOWN )
                    // {
                    //     // Sound + Music
                    //     switch( e.key.key )
                    //     {
                    //         //Play high sound effect
                    //         case SDLK_1:
                    //             Mix_PlayChannel( static_cast<int>( eEffectChannel::High ), gHigh, 0 );
                    //             break;
                    //
                    //         //Play medium sound effect
                    //         case SDLK_2:
                    //             Mix_PlayChannel( static_cast<int>( eEffectChannel::Medium ), gMedium, 0 );
                    //             break;
                    //
                    //         //Play low sound effect
                    //         case SDLK_3:
                    //             Mix_PlayChannel( static_cast<int>( eEffectChannel::Low ), gLow, 0 );
                    //             break;
                    //
                    //         //Play scratch sound effect
                    //         case SDLK_4:
                    //             Mix_PlayChannel( static_cast<int>( eEffectChannel::Scratch ), gScratch, 0 );
                    //             break;
                    //
                    //         //If there is no music playing
                    //         case SDLK_9:
                    //             if( Mix_PlayingMusic() == 0 )
                    //             {
                    //                 //Play the music
                    //                 Mix_PlayMusic( gMusic, -1 );
                    //             }
                    //             //If music is being played
                    //             else
                    //             {
                    //                 //If the music is paused
                    //                 if( Mix_PausedMusic() == 1 )
                    //                 {
                    //                     //Resume the music
                    //                     Mix_ResumeMusic();
                    //                 }
                    //                 //If the music is playing
                    //                 else
                    //                 {
                    //                     //Pause the music
                    //                     Mix_PauseMusic();
                    //                 }
                    //             }
                    //             break;
                    //
                    //         //Stop the music
                    //         case SDLK_0:
                    //             Mix_HaltMusic();
                    //             break;
                    //     }
                    // }
                    //
                    // //Handle button events
                    // for( int i = 0; i < kButtonCount; ++i )
                    // {
                    //     buttons[ i ].handleEvent( &e );
                    // }
                }

                // //Go to next frame
                // frame++;
                //
                // //Cycle animation
                // constexpr int kWakingAnimationFrames = 4;
                // constexpr int kWakingAnimationFramesPerSprite = 6;
                // if( frame / kWakingAnimationFramesPerSprite >= kWakingAnimationFrames )
                // {
                //     frame = 0;
                // }
                //
                // //Set sprite clips
                // constexpr float kSpriteWidth = 64;
                // constexpr float kSpriteHeight = 205;
                // SDL_FRect spriteClips[ kWakingAnimationFrames ] = {
                //     { kSpriteWidth * 0, 0.f, kSpriteWidth, kSpriteHeight },
                //     { kSpriteWidth * 1, 0.f, kSpriteWidth, kSpriteHeight },
                //     { kSpriteWidth * 2, 0.f, kSpriteWidth, kSpriteHeight },
                //     { kSpriteWidth * 3, 0.f, kSpriteWidth, kSpriteHeight }
                // };

                

                //Update square
                // square.move(wall);

                //Rerender text if needed
                // if( renderText )
                // {
                //     //Text is not empty
                //     if( inputText != "" )
                //     {
                //         //Render new text
                //         gInputTextTexture.loadFromRenderedText( inputText.c_str(), kTextColor );
                //     }
                //     //Text is empty
                //     else
                //     {
                //         //Render space texture
                //         gInputTextTexture.loadFromRenderedText( " ", kTextColor );
                //     }
                // }

                //Update dot
                dot.move();

                //Center camera over dot
                camera.x = static_cast<float>( dot.getPosX() + Dot::kDotWidth / 2 - kScreenWidth / 2 );
                camera.y = static_cast<float>( dot.getPosY() + Dot::kDotHeight / 2 - kScreenHeight / 2 );

                //Bound the camera
                if( camera.x < 0 )
                {
                    camera.x = 0;
                }
                else if( camera.x + camera.w > kLevelWidth )
                {
                    camera.x = kLevelWidth - camera.w;
                }
                if( camera.y < 0 )
                {
                    camera.y = 0;
                }
                else if( camera.y + camera.h > kLevelHeight )
                {
                    camera.y = kLevelHeight - camera.h;
                }

                //Fill the background
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF,  0xFF );
                SDL_RenderClear( gRenderer );

                // Render Texture
                gBgTexture.render(0.f, 0.f, &camera);

                //Render dot
                dot.render( camera );


                // gPromptTextTexture.render( ( kScreenWidth - gPromptTextTexture.getWidth() ) / 2.f, ( kScreenHeight - gPromptTextTexture.getHeight() * 2.f ) / 2.f );
                //
                // gInputTextTexture.render( ( kScreenWidth - gInputTextTexture.getWidth() ) / 2.f, ( kScreenHeight - gPromptTextTexture.getHeight() * 2.f ) / 2.f + gPromptTextTexture.getHeight() );


                //Render wall
                // SDL_FRect drawingRect{ static_cast<float>( wall.x ), static_cast<float>( wall.y ),  static_cast<float>( wall.w ),  static_cast<float>( wall.h ) };
                // SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );   
                // SDL_RenderRect( gRenderer, &drawingRect );
                //
                // //Render square
                // square.render();

                //Render dot
                // dot.render();

                //Render current frame
                // SDL_FRect* currentClip{ &spriteClips[ frame / kWakingAnimationFramesPerSprite ] };
                // gSpriteSheetTexture.render( ( kScreenWidth - kSpriteWidth ) / 2, ( kScreenHeight - kSpriteHeight ) / 2, currentClip );
                //
                

                //Update screen
                SDL_RenderPresent(gRenderer);

                //If time remaining in frame
                constexpr Uint64 nsPerFrame = 1000000000 / kScreenFps; 
                Uint64 frameNs{ capTimer.getTicksNS() };
                if ( frameNs < nsPerFrame)
                {
                    SDL_DelayNS(nsPerFrame - frameNs);
                }

                //Fill the background
                // SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                // SDL_RenderClear( gRenderer );

                //Redner text
                // gTextTexture.render( ( kScreenWidth - gTextTexture.getWidth() ) / 2.f, ( kScreenHeight - gTextTexture.getHeight() ) / 2.f );

                //Render buttons
                //Render buttons
                // for( int i = 0; i < kButtonCount; i++ )
                // {
                //     buttons[ i ].render();
                // }

                //Update screen
                // SDL_RenderPresent( gRenderer );
            }

            //Disable text input
            SDL_StopTextInput( gWindow );
        }
    }

    //Clean up
    close();

    return exitCode;
}
