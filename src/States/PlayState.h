#pragma once

#include "State.h"
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
// #include "../Components/TetrisGravityComponent.h"
#include "../Components/BoxColliderComponent.h"
// #include "../Components/MoveComponent.h"
#include "../Components/BoundaryComponent.h"
#include "../Systems/ScoreSystem.h"
// #include "../Shapes/Shape.h"
// #include "../Shapes/LShape.h"


class PlayState : public State
{

    void Enter() override;
    void Exit() override;

private:
    Entity mBackground;

    Entity mBottomBoundCollider;
    Entity mRightBoundCollider;
    Entity mLeftBoundCollider;
    Entity mTopBoundCollider;
};
