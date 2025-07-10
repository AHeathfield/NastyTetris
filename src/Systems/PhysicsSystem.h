#pragma once

#include "../Core/ECS.h"
    #include "../Core/Timer.h"
#include "../Components/TransformComponent.h"
#include "../Components/TetrisGravityComponent.h"

class PhysicsSystem : public System
{
public:
    void Update();
    
private:
    Timer mTimer = Timer();
    float mDeltaTime = 0.8f; // Amount of time before next update
};
