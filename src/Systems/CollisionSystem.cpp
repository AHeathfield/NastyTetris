#include "CollisionSystem.h"
#include <SDL3/SDL_log.h>
#include <string>

extern Coordinator gCoordinator;

void CollisionSystem::UpdateCollisions()
{
    for (const auto& entityA : mEntities)
    {
        for (const auto& entityB : mEntities)
        {
            if (entityA != entityB)
            {
                auto& colliderA = gCoordinator.GetComponent<BoxColliderComponent>(entityA);
                const auto& colliderB = gCoordinator.GetComponent<BoxColliderComponent>(entityB);

                const auto& transformA = gCoordinator.GetComponent<TransformComponent>(entityA);

                // if its collisions in the moving shape ignore them
                // bool isMovingShape = gCoordinator.HasComponent<MoveComponent>(entityA) && gCoordinator.HasComponent<MoveComponent>(entityB);
                // if (checkCollision(colliderA, colliderB) && !isMovingShape)
                if (checkCollision(colliderA, colliderB))
                {
                    checkCollisionSide(colliderA, colliderB);
                    // SDL_Log(std::to_string(colliderA.position.y).c_str());
                    // colliderA.isBottomCollision = isBottomColliding(colliderA, colliderB);
                    // colliderA.position = transformA.position;
                    bool notBoundaryA = !(gCoordinator.HasComponent<BoundaryComponent>(entityA));
                    // if (isBottomColliding(colliderA, colliderB) && notBoundaryA)
                    if (mCollisionSide == BOTTOM && notBoundaryA)
                    {
                        if (gCoordinator.HasComponent<MoveComponent>(entityA))
                        {
                            mRemoveMoveArray.push_back(entityA);
                        }
                        // I think could move colliderA.pos.y = transformA...
                        // colliderA.position.y = colliderB.position.y - 40.f;
                        colliderA.position.y = transformA.position.y;
                    }
                    else if ((mCollisionSide == LEFT || mCollisionSide == RIGHT)  && notBoundaryA)
                    {
                        colliderA.position.x = transformA.position.x;
                    }
                }
            }
        }
    }
}


void CollisionSystem::UpdateTransforms()
{
    for (const auto& entity : mEntities)
    {
        auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);
        const auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);
        
        transform.position = collider.position;
    }
}

void CollisionSystem::UpdateMoveComponents()
{
    for (const auto& entity : mRemoveMoveArray)
    {
        gCoordinator.RemoveComponent<MoveComponent>(entity);
    }
    mRemoveMoveArray.clear();
}

void CollisionSystem::checkCollisionSide(const BoxColliderComponent& a, const BoxColliderComponent& b)
{
    // To help with inaccuracies
    // float spacer = 2.f;

    //Calculate the sides of rect A
    int aMinX = a.position.x;
    int aMaxX = a.position.x + a.w;
    int aMinY = a.position.y;
    int aMaxY = a.position.y + a.h;

    //Calculate the sides of rect B
    int bMinX = b.position.x;
    int bMaxX = b.position.x + b.w;
    int bMinY = b.position.y;
    int bMaxY = b.position.y + b.h;

    // Left Boundary
    if (bMaxX == 760)
    {
        mCollisionSide = LEFT;
        return;
    }
    else if (bMinX == 1160)
    {
        mCollisionSide = RIGHT;
        return;
    }

    if( aMaxY >= bMinY)
    {
        mCollisionSide = BOTTOM;
    }
    else if( aMinX <= bMaxX )
    {
        mCollisionSide = LEFT;
    }
    else if( aMaxX >= bMinX )
    {
        mCollisionSide = RIGHT;
    }
    else if( aMinY <= bMaxY)
    {
        mCollisionSide = TOP;
    }
}

bool CollisionSystem::checkCollision(const BoxColliderComponent& a, const BoxColliderComponent& b)
{
    // To help with inaccuracies
    // float spacer = 2.f;

    //Calculate the sides of rect A
    int aMinX = a.position.x;
    int aMaxX = a.position.x + a.w;
    int aMinY = a.position.y;
    int aMaxY = a.position.y + a.h;

    //Calculate the sides of rect B
    int bMinX = b.position.x;
    int bMaxX = b.position.x + b.w;
    int bMinY = b.position.y;
    int bMaxY = b.position.y + b.h;

    //If left side of A is the right of B
    if( aMinX >= bMaxX )
    {
        return false;
    }

    //If the right side of A to the left of B
    if( aMaxX <= bMinX )
    {
        return false;
    }

    //If the top side of A is below bottom of B
    if( aMinY >= bMaxY)
    {
        return false;
    }

    //If the bottom side of A is above top of B
    if( aMaxY <= bMinY)
    {
        return false;
    }

    return true;
}

bool CollisionSystem::isBottomColliding(const BoxColliderComponent& a, const BoxColliderComponent& b)
{
    // To help with inaccuracies
    // int spacer = 2.f;

    //Calculate the sides of rect A
    int aMinX = a.position.x;
    int aMaxX = a.position.x + a.w;
    int aMinY = a.position.y;
    int aMaxY = a.position.y + a.h;

    //Calculate the sides of rect B
    int bMinX = b.position.x;
    int bMaxX = b.position.x + b.w;
    int bMinY = b.position.y;
    int bMaxY = b.position.y + b.h;

    // Check if its a side wall
    if ((bMaxX < 760) || (bMinX > 1160))
    {
        return false;
    }


    // std::string log = "aMax: " + std::to_string(aMaxY) + " bMax: " + std::to_string(bMaxY);
    // SDL_Log(log.c_str());
    
    return (aMaxY > bMinY);
}
