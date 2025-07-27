#include "PlayShapeSystem.h"
#include <SDL3/SDL_log.h>
#include <string>

extern Coordinator gCoordinator;

void PlayShapeSystem::Update()
{
    auto holdSystem = gCoordinator.GetSystem<HoldSystem>();
    if (currentShape != nullptr)
    {
        // const auto& refTransform = gCoordinator.GetComponent<TransformComponent>(mRefEntity);
        const auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(mRefEntity);

        std::string log = "X: " + std::to_string(currentShape->mShapePos.x) + ", Y: " + std::to_string(currentShape->mShapePos.y);
        // SDL_Log(log.c_str());
        // Updating the shape pos to its proper pos
        // Vector2 newPos = currentShape->mShapePos - (refTransform.position - mRefEntityPosDiff);
        Vector2 newPos = currentShape->mShapePos - (collider.position - mRefEntityPosDiff);
        currentShape->mShapePos = currentShape->mShapePos - newPos;

        // For logs
        // if (holdSystem->spaceWasPressed)
        // {
        //     holdSystem->spaceWasPressed = false;
        //     log = "newPosX: " + std::to_string(newPos.x) + ", newPosY: " + std::to_string(newPos.y);
        //     SDL_Log(log.c_str());
        //
        //     for (const auto& entity : holdSystem->currentShape->GetEntities())
        //     {
        //         const auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);
        //         std::string logc = "collX = " + std::to_string(collider.position.x) + ", collY = " + std::to_string(collider.position.y);
        //         SDL_Log(logc.c_str());
        //     }
        // }
    }
}

void PlayShapeSystem::PlayNextShape(Shape* nextShape)
{
    nextShape->MakeMovable();
    currentShape = nextShape;

    // Used to help update the shape pos
    // mRefEntity = currentShape->GetEntities()[0];
    // Has to be box collider since transform won't be updated yet (so transform is still in holding)
    // mRefEntityPosDiff = gCoordinator.GetComponent<BoxColliderComponent>(mRefEntity).position - currentShape->mShapePos;

    // std::string log = "RefEntityDiffX: " + std::to_string(mRefEntityPosDiff.x) + "Ref...Y: " + std::to_string(mRefEntityPosDiff.y);
    // SDL_Log(log.c_str());
    // log = "X: " + std::to_string(currentShape->mShapePos.x) + ", Y: " + std::to_string(currentShape->mShapePos.y);
    // SDL_Log(log.c_str());
}

void PlayShapeSystem::UpdateRefEntity()
{
    // Used to help update the shape pos
    mRefEntity = currentShape->GetEntities()[0];

    // Has to be box collider since transform won't be updated yet (so transform is still in holding)
    const auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(mRefEntity);
    mRefEntityPosDiff = collider.position - currentShape->mShapePos;

    std::string log = "Collider: " + collider.position.PrintPosition();
    SDL_Log(log.c_str());
    log = "Current Shape Pos: " + currentShape->mShapePos.PrintPosition();
    SDL_Log(log.c_str());
    log = "Ref Entity Pos Diff: " + mRefEntityPosDiff.PrintPosition();
    SDL_Log(log.c_str());
}

bool PlayShapeSystem::isNoPlayingShape()
{
    return mEntities.empty();
}
