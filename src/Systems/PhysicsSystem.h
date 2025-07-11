#pragma once

#include "../Core/ECS.h"
#include "../Core/Timer.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TetrisGravityComponent.h"

// The Physics System will update the entities Collider
class PhysicsSystem : public System
{
public:
    void Update();
};
