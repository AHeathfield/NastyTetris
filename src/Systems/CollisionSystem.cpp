#include "CollisionSystem.h"
#include "PlayShapeSystem.h"
#include <SDL3/SDL_log.h>
#include <string>

extern Coordinator gCoordinator;
extern State* gCurrentState;

void CollisionSystem::UpdateCollisions(Shape* currentShape)
{
    std::set<Entity> moveEntities;
    mNeedRecheck = false;
    mStopMoving = false;
    mCollisionBottom = false;
    mCollisionLeft = false;
    mCollisionRight = false;
    mCollisionTop = false;

    if (mDeathCollision)
    {
        SDL_Log("GAME OVER");
        gCurrentState = new ScoreState();
        mDeathCollision = false;
    }

    for (const auto& entityA : mEntities)
    {
        if (gCoordinator.HasComponent<MoveComponent>(entityA))
        {
            moveEntities.insert(entityA);

            const auto& colliderA = gCoordinator.GetComponent<BoxColliderComponent>(entityA);

            checkEntityCollision(entityA, colliderA, currentShape);
        }
    }

    if (mNeedRecheck)
    {
        // We know it needs to move to side, but we need to double check if it actually needs to move back up
        mCollisionBottom = false;
        mStopMoving = false;
        mMoveMoves.y = 0;

        for (const auto& entityA : moveEntities)
        {

            const auto& colliderA = gCoordinator.GetComponent<BoxColliderComponent>(entityA);
            const auto& transformA = gCoordinator.GetComponent<TransformComponent>(entityA);
            BoxColliderComponent tempColliderA{ Vector2(transformA.position.x, colliderA.position.y), colliderA.w, colliderA.h };
            // log = "tempAColl.x = " + std::to_string(tempColliderA.position.x);
            // SDL_Log(log.c_str());

            // So we don't have repeats of special case, also mMoveMoves.x is already the correct value
            mCollisionRight = false;
            mCollisionLeft = false;

            // This will update the mMoveMoves.y to the proper value
            checkEntityCollision(entityA, tempColliderA, currentShape);
        }
    }

    // Update the components
    for (const auto& entity : moveEntities)
    {
        auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);

        collider.position = collider.position + mMoveMoves;
        if (mStopMoving)
        {
            gCoordinator.RemoveComponent<MoveComponent>(entity);
            gCoordinator.RemoveComponent<TetrisGravityComponent>(entity);

            // Used to check if a line is created
            gCoordinator.AddComponent(entity, RowComponent{});

            if (currentShape != nullptr)
            {
                mDeathCollision = currentShape->mShapePos.y == 140.f;
            }
        }
    }
    mMoveMoves = Vector2();

    // Need to increment rotation if shape rotated and no collision
    if (currentShape != nullptr)
    {
        if (!mIsCollision && currentShape->isRotated)
        {
            currentShape->IncrementRotation();
            currentShape->isRotated = false;
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

void CollisionSystem::checkCollisionSide(const Vector2& aCollider, const Vector2& aCurrentPos, Entity b)//const Vector2& bPos)
{
    Vector2 bPos = gCoordinator.GetComponent<TransformComponent>(b).position;

    // For the boundaries
    if (bPos.y == 940)
    {
        mCollisionBottom = true;
    }
    // else if (bPos.y == 100 && gCoordinator.HasComponent<DeathComponent>(b))
    // {
    //     mDeathCollision = true;
    // }
    else if (bPos.x == 720)
    {
        mCollisionLeft = true;
    }
    else if (bPos.x == 1160)
    {
        mCollisionRight = true;
    }

    // For all colliders
    if (aCollider.y > aCurrentPos.y)
    {
        mCollisionBottom = true;
    }
    if (aCollider.x < aCurrentPos.x)
    {
        mCollisionLeft = true;
    }
    if (aCollider.x > aCurrentPos.x)
    {
        mCollisionRight = true;
    }
    if (aCollider.y < aCurrentPos.y)
    {
        mCollisionTop = true;
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

// ITS GETTING BETTER, FOR NEXT ATTEMPT TRY MAKING IT CHECK ALL THE MOVE ENTITES FIRST FOR A COLLISION!!
void CollisionSystem::checkEntityCollision(Entity entityA, const BoxColliderComponent& colliderA, Shape* currentShape)
{
    for (const auto& entityB : mEntities)
    {
        // if entityB does not have MoveComponent
        if (entityA != entityB && !gCoordinator.HasComponent<MoveComponent>(entityB))
        {
            auto& transformA = gCoordinator.GetComponent<TransformComponent>(entityA);
            const auto& colliderB = gCoordinator.GetComponent<BoxColliderComponent>(entityB);

            if (checkCollision(colliderA, colliderB))
            {
                // SDL_Log("COLLISION");
                // const auto& transformB = gCoordinator.GetComponent<TransformComponent>(entityB);
                checkCollisionSide(colliderA.position, transformA.position, entityB);

                if (currentShape->isRotated)
                {
                    currentShape->Undo();
                }
                else
                {
                    // If we are rechecking this we can just ignore
                    if (mCollisionBottom && (mCollisionLeft || mCollisionRight) && !mNeedRecheck)
                    {
                        mNeedRecheck = true;
                        // std::string log = "aColl.x = " + std::to_string(colliderA.position.x) + ", aColl.y = " + std::to_string(colliderA.position.y) + ", aTrans.x = " + std::to_string(transformA.position.x) + ", aTrans.y = " + std::to_string(transformA.position.y) + "\n bColl.x = " + std::to_string(colliderB.position.x) + ", bColl.y = " + std::to_string(colliderB.position.y);
                        // SDL_Log(log.c_str());
                    }

                    if (mCollisionBottom)
                    {
                        // SDL_Log("BOTTOM!");

                        mMoveMoves.y = -40.f;
                        mStopMoving = true;
                    }
                    if (mCollisionLeft)
                    {
                        // SDL_Log("LEFT!");
                        mMoveMoves.x = 40.f;
                    }
                    if (mCollisionRight)
                    {
                        mMoveMoves.x = -40.f;
                    }
                }
            }
        }
    }
}

