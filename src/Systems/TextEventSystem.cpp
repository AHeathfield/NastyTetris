#include "TextEventSystem.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <cstddef>
#include <string>
// #include <iostream>
// #include <string>

extern Coordinator gCoordinator;

// Helper method
int convertScoreSTRtoINT(std::string scoreSTR)
{
    std::string temp;
    for (int i = 0; i < scoreSTR.length(); i++)
    {
        // SDL_Log(&scoreSTR[i]);
        if (scoreSTR[i] != '0')
        {
            temp += scoreSTR[i];
        }
    }

    if (temp.length() == 0)
    {
        // SDL_Log("LENGTH 0");
        temp = "0";
    }

    int score = std::stoi(temp);
    return score;
}

void TextEventSystem::Init()
{
    mName = "AAA";
    mIsFinalScoreHigh = false;
    auto scoreSystem = gCoordinator.GetSystem<ScoreSystem>();
    // SDL_Log(scoreSystem->GetScoreString().c_str());

    // Opening score file in read mode
    std::ifstream file("src/Scores.txt");
    
    if (!file.is_open())
    {
        SDL_Log("Unable to open file!");
    }

    std::string line;
    // Creates entities to hold the text to render
    int count = 1;
    while (getline(file, line) && count < 11)
    {
        // This places the final score in correct spot
        std::string scoreStr;
        if (scoreStr.substr(0, 2) == "10")
        {
            scoreStr = line.substr(8, 8);
        }
        else
        {
            scoreStr = line.substr(7, 8);
        }

        // Place the mTextField here
        if (scoreSystem->score >= convertScoreSTRtoINT(scoreStr) && mIsFinalScoreHigh == false)
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
                        .text = std::to_string(count) + ". AAA " + scoreSystem->GetScoreString(),
                    });
            gCoordinator.AddComponent(
                    mTextField, 
                    TransformComponent{
                        .position = Vector2(780.f, 260.f + (64 * count))
                    });
            mScores.push_back(mTextField);
            count++;
        }


        std::string nameAndScore;
        if (scoreStr.substr(0, 2) == "10")
        {
            SDL_Log("10");
            nameAndScore = line.substr(4);
        }
        else
        {
            nameAndScore = line.substr(3);
        }

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
                    .text = std::to_string(count) + ". " + nameAndScore,
                });
        gCoordinator.AddComponent(
                score, 
                TransformComponent{
                    .position = Vector2(780.f, 260.f + (64 * count))
                });
        mScores.push_back(score);

        count++;
    }

    // We don't always go to the end of file
    // if (file.eof())
    // {
    //     SDL_Log("REACHED END OF FILE");
    // }
    // else
    // {
    //     SDL_Log("Error reading file");
    // }

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

            if (texture.text.substr(0, 2) == "10")
            {
                texture.text = texture.text.substr(0, 4) + mName + " " + scoreSystem->GetScoreString();
            }
            else
            {
                texture.text = texture.text.substr(0, 3) + mName + " " + scoreSystem->GetScoreString();
            }
            auto renderSystem = gCoordinator.GetSystem<RenderSystem>();
            renderSystem->LoadMedia(&texture);
        }
    }
}

void TextEventSystem::Close()
{
    // Saves the score to file
    Save();

    gCoordinator.GetComponent<TextureComponent>(mFinalScore).destroy();
    gCoordinator.DestroyEntity(mFinalScore);

    for (const auto& entity : mScores)
    {
        gCoordinator.GetComponent<TextureComponent>(entity).destroy();
        gCoordinator.DestroyEntity(entity);
    }

    mScores.clear();
}


// Private methods
void TextEventSystem::Save()
{
    // Opening file
    std::ofstream file("src/Scores.txt");

    for (int i = 0; i < mScores.size(); i++)
    {
        const auto& texture = gCoordinator.GetComponent<TextureComponent>(mScores[i]);
        file << texture.text << std::endl;
    }

    file.close();
}
