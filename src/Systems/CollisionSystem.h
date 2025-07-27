#pragma once

#include "../Core/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/MoveComponent.h"
#include "../Components/BoundaryComponent.h"
#include "../Components/TetrisGravityComponent.h"
#include "../Components/RowComponent.h"
#include "../Shapes/Shape.h"
#include <set>

// The collision system will update the entities transform
class CollisionSystem : public System
{
public:
    void UpdateCollisions(Shape* currentShape);
    void UpdateTransforms();
    void UpdateMoveComponents();

    bool checkCollision(const BoxColliderComponent& a, const BoxColliderComponent& b);
    
    // Made them vectors but orignally it was BoxCollider, Transform, Transform
    void checkCollisionSide(const Vector2& aCollider, const Vector2& aCurrentPos, const Vector2& bPos);
    // bool isBottomColliding(const BoxColliderComponent& a, const BoxColliderComponent& b);

    void checkEntityCollision(Entity entityA, const BoxColliderComponent& colliderA, Shape* currentShape);

private:

    bool mStopMoving = false;
    bool mIsCollision = false;
    bool mNeedRecheck = false;
    bool mCollisionLeft = false;
    bool mCollisionRight = false;
    bool mCollisionBottom = false;

    std::set<Entity> mMoveSet;
    Vector2 mMoveMoves = Vector2();
    bool mRemoveMoves = false;
};
