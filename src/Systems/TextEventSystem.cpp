#include "TextEventSystem.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>

extern Coordinator gCoordinator;


void TextEventSystem::Init()
{
    auto scoreSystem = gCoordinator.GetSystem<ScoreSystem>();

    // Opening score file in read mode
    std::ifstream file("src/Scores.txt");
    std::string line;
    
    // Creates entities to hold the text to render
    int count = 1;
    while (getline(file, line))
    {
        // SDL_Log(line.c_str());
        Entity score = gCoordinator.CreateEntity();
        score = gCoordinator.CreateEntity();
        gCoordinator.AddComponent(
                score,
                TextureComponent{
                    .texture = nullptr,
                    .path = "src/Assets/8bit16.ttf",
                    .isText = true,
                    .fontSize = 32,
                    .text = line,
                });
        gCoordinator.AddComponent(
                score, 
                TransformComponent{
                    .position = Vector2(780.f, 260.f + (64 * count))
                });
        mScores.push_back(score);

        count++;
    }

    if (file.eof())
    {
        SDL_Log("REACHED END OF FILE");
    }
    else
    {
        SDL_Log("Error reading file");
    }

    file.close();


    // Test creating writable text
    mTextField = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mTextField,
            TextureComponent{
                .texture = nullptr,
                .path = "src/Assets/8bit16.ttf",
                .isText = true,
                .fontSize = 64,
                .text = "AAA",
            });
    gCoordinator.AddComponent(
            mTextField, 
            TransformComponent{
                // .position = Vector2(888.f, 260.f)
                .position = Vector2(58.f, 260.f)
            });

    // Displaying final score
    mFinalScore = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mFinalScore,
            TextureComponent{
                .texture = nullptr,
                .path = "src/Assets/8bit16.ttf",
                .isText = true,
                .fontSize = 64,
                .text = scoreSystem->GetScore(),
            });
    gCoordinator.AddComponent(
            mFinalScore, 
            TransformComponent{
                .position = Vector2(774.f, 260.f)
            });
}

void TextEventSystem::HandleEvent(SDL_Event e)
{
    auto& texture = gCoordinator.GetComponent<TextureComponent>(mTextField);

    // Special Key input
    if (e.type == SDL_EVENT_KEY_DOWN)
    {
        // For Deleting
        if (e.key.key == SDLK_BACKSPACE && texture.text.length() > 0)
        {
            texture.text.pop_back();
            auto renderSystem = gCoordinator.GetSystem<RenderSystem>();
            renderSystem->LoadMedia(&texture);
        }
    }

    // Text input event
    else if (e.type == SDL_EVENT_TEXT_INPUT)
    {
        //If not copying or pasting
        char firstChar{ static_cast<char> ( toupper( e.text.text[ 0 ] ) ) };
        if( !( SDL_GetModState() & SDL_KMOD_CTRL && ( firstChar == 'C' || firstChar == 'V' ) ) )
        {
            //Append character if length is less than 3
            if (texture.text.length() < 3)
            {
                texture.text += e.text.text;
            }

            auto renderSystem = gCoordinator.GetSystem<RenderSystem>();
            renderSystem->LoadMedia(&texture);
        }
    }
}

void TextEventSystem::Close()
{
    gCoordinator.GetComponent<TextureComponent>(mTextField).destroy();
    gCoordinator.DestroyEntity(mTextField);
}
