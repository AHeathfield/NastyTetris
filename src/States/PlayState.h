#pragma once

#include "State.h"
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"

class PlayState : public State
{
public:
    void Enter() override;
    void Exit() override;

private:
    Entity mBackground;
};
