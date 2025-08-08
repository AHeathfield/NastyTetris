#include "HoldSystem.h"
#include <string>

extern Coordinator gCoordinator;

void HoldSystem::Update()
{
    auto playShapeSystem = gCoordinator.GetSystem<PlayShapeSystem>();
    Vector2 holdPos = Vector2(400.f, 260.f);

    // canHoldShape makes sure the shape is only held once
    if (currentShape != nullptr && playShapeSystem->canHoldShape)
    {
        if (canHold)//mEntities.empty()) //&& !holdPos.equals(currentShape->mShapePos))
        {
            spaceWasPressed = true;
            playShapeSystem->UpdateRefEntity();
            currentShape->SetPosition(holdPos);

            currentShape->RemoveMovable();
            playShapeSystem->Update();
            canHold = false;
            // mCanRelease = true;

            // Move the current Shape to hold area and make it not move 

            // std::string log = "holdX = " + std::to_string(currentShape->mShapePos.x) + ", holdY = " + std::to_string(currentShape->mShapePos.y);
            // SDL_Log(log.c_str());
            // SDL_Log("Removing Movable and moving current to correct spot");

        }

        if (canRelease)
        {
            // SDL_Log("Releasing");
            Vector2 playPos = Vector2(880.f, 140.f);

            // for (const auto& entity : playShapeSystem->currentShape->GetEntities())
            // {
            //     gCoordinator.RemoveComponent<HoldComponent>(entity);
            // }

            oldShape->MakeMovable();
            playShapeSystem->currentShape = oldShape;
            playShapeSystem->UpdateRefEntity();
            playShapeSystem->currentShape->SetPosition(playPos);
            oldShape = nullptr;
            canRelease = false;

            // This is so you can only swap shapes 1 time per shape
            playShapeSystem->canHoldShape = false;
        }
    }
}

void HoldSystem::HoldShape(Shape* newShape)
{
    oldShape = currentShape;
    // SDL_Log("Holding");
    currentShape = newShape;
}

void HoldSystem::Close()
{
    if (currentShape != nullptr)
    {
        currentShape->Destroy();
        currentShape = nullptr;
    }
    if (oldShape != nullptr)
    {
        oldShape->Destroy();
        oldShape = nullptr;
    }
}
