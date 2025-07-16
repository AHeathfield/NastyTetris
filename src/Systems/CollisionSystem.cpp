#include "CollisionSystem.h"
#include <SDL3/SDL_log.h>
#include <string>

extern Coordinator gCoordinator;

void CollisionSystem::UpdateCollisions(Shape* currentShape)
{
    // const std::set<Entity> entitiesCopy = mEntities;
    std::set<Entity> moveEntities;
    bool isCollision = false;
    bool stopMoving = false;

    for (const auto& entityA : mEntities)
    {
        if (gCoordinator.HasComponent<MoveComponent>(entityA))
        {
            moveEntities.insert(entityA);

            if (!isCollision)
            {
                for (const auto& entityB : mEntities)
                {
                    // if entityB does not have MoveComponent
                    if (entityA != entityB && !gCoordinator.HasComponent<MoveComponent>(entityB))
                    {
                        auto& colliderA = gCoordinator.GetComponent<BoxColliderComponent>(entityA);
                        const auto& colliderB = gCoordinator.GetComponent<BoxColliderComponent>(entityB);

                        isCollision = checkCollision(colliderA, colliderB);
                        if (isCollision)
                        {
                            // SDL_Log("Collision!");
                            checkCollisionSide(colliderA, colliderB);
                            if (mCollisionSide == BOTTOM)
                            {
                                mMoveMoves.y = -40.f;
                                stopMoving = true;
                            }
                            else if (mCollisionSide == LEFT)
                            {
                                mMoveMoves.x = 40.f;
                                break;
                            }
                            else if (mCollisionSide == RIGHT)
                            {
                                mMoveMoves.x = -40.f;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    // Update the components
    for (const auto& entity : moveEntities)
    {
        auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);
        collider.position = collider.position + mMoveMoves;
        
        if (stopMoving)
        {
            gCoordinator.RemoveComponent<MoveComponent>(entity);
            gCoordinator.RemoveComponent<TetrisGravityComponent>(entity);
        }
    }
    mMoveMoves = Vector2();

    // Need to increment rotation if shape rotated and no collision
    if (currentShape != nullptr)
    {
        if (!isCollision && currentShape->isRotated)
        {
            currentShape->IncrementRotation();
            currentShape->isRotated = false;
        }
    }
}

// void CollisionSystem::UpdateCollisions()
// {
//     bool isCollision = false;
//     for (const auto& entityA : mEntities)
//     {
//         for (const auto& entityB : mEntities)
//         {
//             if (entityA != entityB)
//             {
//                 auto& colliderA = gCoordinator.GetComponent<BoxColliderComponent>(entityA);
//                 const auto& colliderB = gCoordinator.GetComponent<BoxColliderComponent>(entityB);
//
//                 const auto& transformA = gCoordinator.GetComponent<TransformComponent>(entityA);
//
//                 // bool isMovingShape = gCoordinator.HasComponent<MoveComponent>(entityA) && gCoordinator.HasComponent<MoveComponent>(entityB);
//                 // Current problem is that it doesn't remove all the MoveComponents I think
//                 isCollision = checkCollision(colliderA, colliderB);
//                 if (isCollision)
//                 {
//
//                     checkCollisionSide(colliderA, colliderB);
//                     // SDL_Log(std::to_string(colliderA.position.y).c_str());
//                     // colliderA.isBottomCollision = isBottomColliding(colliderA, colliderB);
//                     // colliderA.position = transformA.position;
//                     bool notBoundaryA = !(gCoordinator.HasComponent<BoundaryComponent>(entityA));
//                     // if (isBottomColliding(colliderA, colliderB) && notBoundaryA)
//                     if (mCollisionSide == BOTTOM && notBoundaryA)
//                     {
//                         if (gCoordinator.HasComponent<MoveComponent>(entityA))
//                         {
//                             // mMoveSet.insert(entityA);
//                             SDL_Log("Bottom Collision");
//                             mRemoveMoves = true;
//                             mMoveMoves.y = -40.f;
//                             break;
//                         }
//                         // colliderA.position.y = transformA.position.y;
//                     }
//                     // else if ((mCollisionSide == LEFT || mCollisionSide == RIGHT)  && notBoundaryA)
//                     // {
//                     //     colliderA.position.x = transformA.position.x;
//                     // }
//                     else if (mCollisionSide == LEFT && notBoundaryA)
//                     {
//                         mMoveMoves.x = 40.f;
//                         break;
//                     }
//                     else if (mCollisionSide == RIGHT && notBoundaryA)
//                     {
//                         mMoveMoves.x = -40.f;
//                         break;
//                     }
//                 }
//             }
//         }
//         if (mRemoveMoves || isCollision)
//         {
//             break;
//         }
//     }
// }


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
    // Fills the set with all the entities with MoveComponent
    for (const auto& entity : mEntities)
    {
        if (gCoordinator.HasComponent<MoveComponent>(entity))
        {
            mMoveSet.insert(entity);
        }
    }

    for (const auto& entity : mMoveSet)
    {
        auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);
        collider.position = collider.position + mMoveMoves;

        if (mRemoveMoves)
        {
            gCoordinator.RemoveComponent<MoveComponent>(entity);
            gCoordinator.RemoveComponent<TetrisGravityComponent>(entity);
        }
    }
    mRemoveMoves = false;
    mMoveSet.clear();
    mMoveMoves = Vector2();
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
