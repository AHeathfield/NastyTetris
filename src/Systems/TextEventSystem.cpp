#include "TextEventSystem.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <cstddef>
// #include <iostream>
#include <string>

extern Coordinator gCoordinator;

// Helper method
// int convertScoreSTRtoINT(std::string scoreSTR)
// {
//     std::string temp;
//     for (int i = 0; i < scoreSTR.length(); i++)
//     {
//         if (!scoreSTR[i] == '0')
//         {
//             temp = scoreSTR[i];
//         }
//     }
//
//     return std::stoi(temp);
// }

void TextEventSystem::Init()
{
    mName = "AAA";
    mIsFinalScoreHigh = false;
    auto scoreSystem = gCoordinator.GetSystem<ScoreSystem>();

    // Opening score file in read mode
    std::ifstream file("src/Scores.txt");
    std::string line;
    
    // Creates entities to hold the text to render
    int count = 1;
    while (getline(file, line) && count < 11)
    {
        // This places the final score in correct spot
        std::string scoreStr = line.substr(4, 8);

        // Place the mTextField here
        if (scoreSystem->score >= std::stoi(scoreStr) && mIsFinalScoreHigh == false)
        {
            mIsFinalScoreHigh = true;
            // SDL_Log("IT WORKED");
            // Creating the editable text
            mTextField = gCoordinator.CreateEntity();
            gCoordinator.AddComponent(
                    mTextField,
                    TextureComponent{
                        .texture = nullptr,
                        .path = "src/Assets/8bit16.ttf",
                        .isText = true,
                        .fontSize = 32,
                        .text = std::to_string(count) + ". AAA " + scoreStr,
                    });
            gCoordinator.AddComponent(
                    mTextField, 
                    TransformComponent{
                        // .position = Vector2(888.f, 260.f)
                        .position = Vector2(780.f, 260.f + (64 * count))
                    });
            mScores.push_back(mTextField);
        }
        else
        {

            // This creates the text entity for the score
            Entity score = gCoordinator.CreateEntity();
            score = gCoordinator.CreateEntity();
            gCoordinator.AddComponent(
                    score,
                    TextureComponent{
                        .texture = nullptr,
                        .path = "src/Assets/8bit16.ttf",
                        .isText = true,
                        .fontSize = 32,
                        .text = std::to_string(count) + ". " + line,
                    });
            gCoordinator.AddComponent(
                    score, 
                    TransformComponent{
                        .position = Vector2(780.f, 260.f + (64 * count))
                    });
            mScores.push_back(score);
        }

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


    // Displaying final score
    mFinalScore = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mFinalScore,
            TextureComponent{
                .texture = nullptr,
                .path = "src/Assets/8bit16.ttf",
                .isText = true,
                .fontSize = 64,
                .text = scoreSystem->GetScoreString(),
            });
    gCoordinator.AddComponent(
            mFinalScore, 
            TransformComponent{
                .position = Vector2(774.f, 260.f)
            });
}

void TextEventSystem::HandleEvent(SDL_Event e)
{
    // If the score is in top 10
    if (mIsFinalScoreHigh)
    {
        auto& texture = gCoordinator.GetComponent<TextureComponent>(mTextField);
        bool isNameChanged = false;

        // Special Key input
        if (e.type == SDL_EVENT_KEY_DOWN)
        {
            // For Deleting
            if (e.key.key == SDLK_BACKSPACE && mName.length() > 0)
            {
                mName.pop_back();
                isNameChanged = true;
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
                if (mName.length() < 3)
                {
                    mName += e.text.text;
                    isNameChanged = true;
                }
            }
        }

        // Updating the texture
        if (isNameChanged)
        {
            auto scoreSystem = gCoordinator.GetSystem<ScoreSystem>();
            texture.text = texture.text.substr(0, 3) + mName + " " + scoreSystem->GetScoreString();
            auto renderSystem = gCoordinator.GetSystem<RenderSystem>();
            renderSystem->LoadMedia(&texture);
        }
    }
}

void TextEventSystem::Close()
{
    gCoordinator.GetComponent<TextureComponent>(mFinalScore).destroy();
    gCoordinator.DestroyEntity(mFinalScore);

    for (const auto& entity : mScores)
    {
        gCoordinator.GetComponent<TextureComponent>(entity).destroy();
        gCoordinator.DestroyEntity(entity);
    }

    mScores.clear();
}
