#pragma once

#include "State.h"
// #include "../Core/ECS.h"
// #include "../Core/Vector2.h"
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/Buttons/ButtonComponent.h"
#include "../Components/Buttons/StateChangerButton.h"

class TitleState : public State
{
public:
    void Enter() override;
    void Exit() override;

private:
    // Entities
    Entity mBackground;
    Entity mPlayButton;
};
