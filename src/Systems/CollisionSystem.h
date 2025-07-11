#pragma once

#include "../Core/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

// The collision system will update the entities transform
class CollisionSystem : public System
{
public:
    void UpdateCollisions();
    void UpdateTransforms();

    bool checkCollision(const BoxColliderComponent& a, const BoxColliderComponent& b);
};
