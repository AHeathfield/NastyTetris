#pragma once

#include "../Core/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/MoveComponent.h"
#include "../Components/BoundaryComponent.h"
#include <vector>

// The collision system will update the entities transform
class CollisionSystem : public System
{
public:
    void UpdateCollisions();
    void UpdateTransforms();
    void UpdateMoveComponents();

    bool checkCollision(const BoxColliderComponent& a, const BoxColliderComponent& b);
    void checkCollisionSide(const BoxColliderComponent& a, const BoxColliderComponent& b);
    bool isBottomColliding(const BoxColliderComponent& a, const BoxColliderComponent& b);

private:
    enum CollisionSide {
        TOP,
        RIGHT,
        BOTTOM,
        LEFT
    };

    enum CollisionSide mCollisionSide;
    std::vector<Entity> mRemoveMoveArray;
};
