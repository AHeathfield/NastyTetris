#pragma once

#include "../Core/ECS.h"
#include "../Components/ScoreComponent.h"
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
#include "RenderSystem.h"
#include <string>

class ScoreSystem : public System
{
public:
    void Init();
    void Update();
    void Close();

    std::string GetScore();

    int score;
    int level;
    int lines;

private:
    Entity mScore, mLevel, mLines;
};
