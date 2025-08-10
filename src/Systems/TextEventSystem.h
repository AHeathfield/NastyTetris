#pragma once

#include <iostream>
#include <fstream>

#include "../Core/ECS.h"
#include <SDL3/SDL_events.h>
#include <string>

#include "../Components/BoxColliderComponent.h"
#include "../Components/MoveComponent.h"
#include "../Components/HoldComponent.h"
#include "../Shapes/Shape.h"
#include "../Systems/ScoreSystem.h"
#include "../Systems/RenderSystem.h"

class TextEventSystem : public System
{
public:
    void Init();
    void HandleEvent(SDL_Event e);
    void Close();

private:
    void Save();

private:
    Entity mFinalScore;
    Entity mTextField;
    std::vector<Entity> mScores;
    std::string mName = "AAA";

    bool mIsFinalScoreHigh = false;
};
