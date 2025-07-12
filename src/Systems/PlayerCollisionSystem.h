#pragma once

#include "../Core/ECS.h"
#include "../Components/MoveComponent.h"
#include "../Components/BoxColliderComponent.h"

class PlayerCollisionSystem : public System
{
public:
    void Update();
};
