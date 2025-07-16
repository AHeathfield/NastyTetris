#include "PlayerEventSystem.h"

extern Coordinator gCoordinator;

void PlayerEventSystem::HandleEvent(SDL_Event e, Shape* currentShape)
{
    if (e.type == SDL_EVENT_KEY_DOWN)
    {
        if (e.key.key == SDLK_UP && currentShape != nullptr)
        {
            SDL_Log("Rotating");
            currentShape->Rotate();
            currentShape->isRotated = true;
        }

        else
        {
            for (const auto& entity : mEntities)
            {
                // TODO: Use command pattern!
                if (e.key.key == SDLK_LEFT)
                {
                    auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);
                    const auto& moveComponent = gCoordinator.GetComponent<MoveComponent>(entity);

                    collider.position.x -= moveComponent.distance;
                }
                else if (e.key.key == SDLK_RIGHT)
                {
                    auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);
                    const auto& moveComponent = gCoordinator.GetComponent<MoveComponent>(entity);

                    collider.position.x += moveComponent.distance;
                }
                else if (e.key.key == SDLK_DOWN)
                {
                    auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);
                    const auto& moveComponent = gCoordinator.GetComponent<MoveComponent>(entity);

                    collider.position.y += moveComponent.distance;
                }
            }
        }
    }
}
