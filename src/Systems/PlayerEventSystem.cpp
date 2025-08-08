#include "PlayerEventSystem.h"
#include <SDL3/SDL_keycode.h>

extern Coordinator gCoordinator;

void PlayerEventSystem::HandleEvent(SDL_Event e)
{
    auto playShapeSystem = gCoordinator.GetSystem<PlayShapeSystem>();
    if (e.type == SDL_EVENT_KEY_DOWN)
    {
        if (e.key.key == SDLK_UP && playShapeSystem->currentShape != nullptr)
        {
            playShapeSystem->currentShape->Rotate();
            playShapeSystem->currentShape->isRotated = true;
        }

        else
        {
            if (e.key.key == SDLK_SPACE && playShapeSystem->canHoldShape)
            {
                bool rotated = false;
                // Unrotates the shape
                while (playShapeSystem->currentShape->mCurrentRotationDegrees != 0)
                {
                    rotated = true;
                    playShapeSystem->currentShape->Rotate();
                    playShapeSystem->currentShape->IncrementRotation();
                }

                // Updating the shapePos after rotating
                if (rotated)
                {
                    playShapeSystem->Update();
                }

                // Adding HoldComponent to the shape if it doesn't have it already

                auto holdSystem = gCoordinator.GetSystem<HoldSystem>();

                Shape* temp = nullptr;
                if (holdSystem->currentShape != nullptr)
                {
                    holdSystem->canRelease = true;
                }
                holdSystem->canHold = true;
                holdSystem->HoldShape(playShapeSystem->currentShape);
            }

            bool scoreAdded = false;
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

                    if (scoreAdded == false)
                    {
                        auto scoreSystem = gCoordinator.GetSystem<ScoreSystem>();
                        scoreSystem->score += 1;
                        scoreSystem->Update();

                        scoreAdded = true;
                    }
                }
            }
        }
    }
}
