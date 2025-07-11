#pragma once

#include "State.h"
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/TetrisGravityComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/MoveComponent.h"


class PlayState : public State
{
public:
    void Enter() override;
    void Exit() override;

private:
    Entity mBackground;
    Entity mTestBlock;
    Entity mBottomBoundCollider;
    Entity mRightBoundCollider;
    Entity mLeftBoundCollider;
};
