#include "PhysicsSystem.h"

extern Coordinator gCoordinator;

void PhysicsSystem::Update()
{
    for (const auto& entity : mEntities)
    {
        auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);
        const auto& gravityComponent = gCoordinator.GetComponent<TetrisGravityComponent>(entity);

        collider.position.y += gravityComponent.gravity;
    }
}
