#include "PlayerCollisionSystem.h"
#include <vector>

extern Coordinator gCoordinator;

void PlayerCollisionSystem::Update()
{
    std::vector<Entity> removeArray;
    for (const auto& entity : mEntities)
    {
        const auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);

        // Remove the move component so player can no longer move that block with arrow keys
        if (collider.isBottomCollision)
        {
            removeArray.push_back(entity);
        }
    }

    for (const auto& entity : removeArray)
    {
        gCoordinator.RemoveComponent<MoveComponent>(entity);
    }
}
