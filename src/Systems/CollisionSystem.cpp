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
                // auto& transformB = gCoordinator.GetComponent<TransformComponent>(entityB);

                if (checkCollision(colliderA, colliderB))
                {
                    colliderA.isBottomCollision = isBottomColliding(colliderA, colliderB);
                    colliderA.position = transformA.position;
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


bool CollisionSystem::checkCollision(const BoxColliderComponent& a, const BoxColliderComponent& b)
{
    // To help with inaccuracies
    float spacer = 2.f;

    //Calculate the sides of rect A
    float aMinX = a.position.x;
    float aMaxX = a.position.x + a.w;
    float aMinY = a.position.y;
    float aMaxY = a.position.y + a.h;

    //Calculate the sides of rect B
    float bMinX = b.position.x;
    float bMaxX = b.position.x + b.w;
    float bMinY = b.position.y;
    float bMaxY = b.position.y + b.h;

    //If left side of A is the right of B
    if( aMinX > bMaxX - spacer )
    {
        return false;
    }

    //If the right side of A to the left of B
    if( aMaxX < bMinX + spacer)
    {
        return false;
    }

    //If the top side of A is below bottom of B
    if( aMinY > bMaxY - spacer)
    {
        return false;
    }

    //If the bottom side of A is above top of B
    if( aMaxY < bMinY + spacer)
    {
        return false;
    }

    return true;
}

bool CollisionSystem::isBottomColliding(const BoxColliderComponent& a, const BoxColliderComponent& b)
{
    // To help with inaccuracies
    float spacer = 2.f;

    //Calculate the sides of rect A
    float aMinX = a.position.x;
    float aMaxX = a.position.x + a.w;
    float aMinY = a.position.y;
    float aMaxY = a.position.y + a.h;

    //Calculate the sides of rect B
    float bMinX = b.position.x;
    float bMaxX = b.position.x + b.w;
    float bMinY = b.position.y;
    float bMaxY = b.position.y + b.h;

    // Check if its a side wall
    if ((bMaxX - spacer < 760.f) || (bMinX + spacer > 1160.f))
    {
        return false;
    }


    // std::string log = "aMax: " + std::to_string(aMaxY) + " bMax: " + std::to_string(bMaxY);
    // SDL_Log(log.c_str());

    return (aMaxY - spacer > bMinY);
}
