#include "RowSystem.h"
#include <SDL3/SDL_log.h>
#include <string>
#include <unordered_map>
#include <vector>

extern Coordinator gCoordinator;

// Helper method
// THIS SHOULD BE ALIGNED WITH THE AMOUNT OF TIME IT TAKES TO FALL
// Think about removing the RowCOmponent
void addToDeleteRows(const std::vector<Entity>& row, std::vector<Entity>& blocksToDelete)
{
    for (const auto& entity : row)
    {
        // Changing texture to white sprite
        auto& texture = gCoordinator.GetComponent<TextureComponent>(entity);
        texture.spriteClip.x = 120.f;
        texture.spriteClip.y = 40.f;

        gCoordinator.RemoveComponent<RowComponent>(entity);
        blocksToDelete.push_back(entity);
    }
}
    // auto renderSystem = gCoordinator.GetSystem<RenderSystem>();
    // // Chaning it to white sprite when its a row
    // for (const auto& entity : row)
    // {
    //     auto& texture = gCoordinator.GetComponent<TextureComponent>(entity);
    //     texture.spriteClip.x = 120.f;
    //     texture.spriteClip.y = 40.f;
    // }
    //
    // renderSystem->Update();
    // Timer animationTimer = Timer();
    // float animationTime = 1.f;
    // animationTimer.start();
    //
    // // This is like purposely creating a lagspike, which is actually how its like in the real tetris
    // while (animationTimer.getTimeS() < animationTime)
    // {
    //     SDL_Log("Waiting");
    // }

//     for (const auto& entity : row)
//     {
//         gCoordinator.DestroyEntity(entity);
//     }
// }

void RowSystem::Update()
{
    // Updating the grid
    for (const auto& entity : mEntities)
    {
        const auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);

        // if there is no key with this y yet
        if (mGrid.find(transform.position.y) == mGrid.end())
        {
            std::vector<Entity> row = {entity};
            mGrid.insert({transform.position.y, row});
        }
        else
        {
            mGrid.at(transform.position.y).push_back(entity);
        }
    }

    // Moving all down if row was deleted
    if (mCanMoveDown)
    {
        const float blockHeight = 40.f;
        if (mRowsRemoved != 0)
        {
            for (auto row = mGrid.begin(); row != mGrid.end(); row++)
            {
                for (const auto& block : row->second)
                {
                    // SDL_Log("Moving Down");
                    auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(block);
                    auto& transform = gCoordinator.GetComponent<TransformComponent>(block);

                    collider.position.y += blockHeight * mRowsRemoved;
                    transform.position.y += blockHeight * mRowsRemoved;
                }
            }
            mRowsRemoved = 0;
        }

        mCanMoveDown = false;
    }

    // Deleting Rows
    const int rowLength = 10;
    std::unordered_map<float, std::vector<Entity>> mGridCopy = mGrid;
    for (auto row = mGridCopy.begin(); row != mGridCopy.end(); row++)
    {
        if (row->second.size() == rowLength)
        {
            // Delete row
            // std::string log = "Delete row: " + std::to_string(row->first);
            // SDL_Log(log.c_str());
            addToDeleteRows(row->second, mBlocksToDelete);
            mGrid.erase(row->first);
            mRowsRemoved++;
        }
        else
        {
            // Clear the vector
            mGrid.at(row->first).clear();
        }
    }
}


void RowSystem::DeleteRows()
{
    for (const auto& block : mBlocksToDelete)
    {
        gCoordinator.DestroyEntity(block);
    }
    mBlocksToDelete.clear();
    mCanMoveDown = true;
}

// void RowSystem::MoveBlocksDown()
// {
//     // Moving all down if row was deleted
//     const float blockHeight = 40.f;
//     for (auto row = mGrid.begin(); row != mGrid.end(); row++)
//     {
//         for (const auto& block : row->second)
//         {
//             auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(block);
//             auto& transform = gCoordinator.GetComponent<TransformComponent>(block);
//
//             collider.position.y += blockHeight * mRowsRemoved;
//             transform.position.y += blockHeight * mRowsRemoved;
//         }
//     }
//     mRowsRemoved = 0;
// }

