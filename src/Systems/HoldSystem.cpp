#include "HoldSystem.h"
#include <string>

extern Coordinator gCoordinator;

void HoldSystem::Update()
{
    auto playShapeSystem = gCoordinator.GetSystem<PlayShapeSystem>();
    Vector2 holdPos = Vector2(400.f, 260.f);

    if (currentShape != nullptr)
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

            std::string log = "holdX = " + std::to_string(currentShape->mShapePos.x) + ", holdY = " + std::to_string(currentShape->mShapePos.y);
            SDL_Log(log.c_str());
            SDL_Log("Removing Movable and moving current to correct spot");

        }

        if (canRelease)
        {
            SDL_Log("Releasing");
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
        }

        

        
    }
}

void HoldSystem::HoldShape(Shape* newShape)
{
    oldShape = currentShape;
    SDL_Log("Holding");
    currentShape = newShape;
}

// Shape* HoldSystem::ReleaseShape()
// {
//     // Problem is that it releases the shape first then we basically immediately put it back in
//     // Also when we release shape and put new one in that one for some reason is still the currentShape
//     // if (mCanRelease)
//     // {
//     //     oldShape = currentShape;
//     //
//     //     SDL_Log("Releasing");
//     //     Vector2 playPos = Vector2(880.f, 140.f);
//     //     currentShape->SetPosition(playPos);
//     //
//     //     for (const auto& entity : currentShape->GetEntities())
//     //     {
//     //         gCoordinator.RemoveComponent<HoldComponent>(entity);
//     //     }
//     //
//     //     currentShape->MakeMovable();
//     //     currentShape = nullptr;
//     // }
//     // else
//     // {
//     //     SDL_Log("Cannot release");
//     // }
//     //
//     // return oldShape;
// }
