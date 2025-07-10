#include "PhysicsSystem.h"

extern Coordinator gCoordinator;

void PhysicsSystem::Update()
{
    if (mTimer.isStarted())
    {
        if (mTimer.getTimeS() > mDeltaTime)
        {
            mTimer.stop();
            for (const auto& entity : mEntities)
            {
                auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);
                auto& gravityComponent = gCoordinator.GetComponent<TetrisGravityComponent>(entity);

                transform.position.y += gravityComponent.gravity;

                // auto* pTransform = &transform;
                // pTransform->position.y += gravityComponent.gravity;
            }
        }
    }
    else
    {
        mTimer.start();
    }

}
