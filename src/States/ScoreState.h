#pragma once

#include "State.h"
// #include "../Core/ECS.h"
// #include "../Core/Vector2.h"
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/Buttons/ButtonComponent.h"
#include "../Components/Buttons/StateChangerButton.h"
#include "../Systems/TextEventSystem.h"
#include "../Systems/RenderSystem.h"

class ScoreState : public State
{
public:
    void Enter() override;
    void Exit() override;

private:
    // Entities
    Entity mBackground;
    Entity mPlayButton;
};
