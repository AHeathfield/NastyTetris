#include "RowSystem.h"
#include <SDL3/SDL_log.h>
#include <string>
#include <unordered_map>
#include <vector>

extern Coordinator gCoordinator;

// Just for the rowsfx
void RowSystem::Init()
{
    mRowSFX = gCoordinator.CreateEntity();
    std::unordered_map<std::string, Audio> entityAudios;
    Audio rowSFX = {
        .filePath = "src/Assets/SFX/RowSFX.wav",
    };
    entityAudios.insert({"Row", rowSFX});
    gCoordinator.AddComponent(
            mRowSFX,
            AudioComponent{
                .audios = entityAudios
            });
}

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
                // To make sure it doesnt move rows below the one that was removed
                if (row->first < mLowestRowRemovedY)
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
            }
            mRowsRemoved = 0;
            mLowestRowRemovedY = 0;
        }

        mCanMoveDown = false;
    }

    // Deleting Rows
    const int rowLength = 10;
    bool isDeletion = false;
    std::unordered_map<float, std::vector<Entity>> mGridCopy = mGrid;
    for (auto row = mGridCopy.begin(); row != mGridCopy.end(); row++)
    {
        if (row->second.size() == rowLength)
        {
            // To make sure no rows moved are rows below note +y is down
            if (row->first > mLowestRowRemovedY)
            {
                mLowestRowRemovedY = row->first;
            }

            // Delete row
            addToDeleteRows(row->second, mBlocksToDelete);
            mGrid.erase(row->first);
            mRowsRemoved++;
            isDeletion = true;
        }
        else
        {
            // Clear the vector
            mGrid.at(row->first).clear();
        }
    }

    if (isDeletion)
    {
        // Playing sound effect for deleted row
        gCoordinator.GetComponent<AudioComponent>(mRowSFX).Play("Row");
    }
}


void RowSystem::DeleteRows()
{
    const int rowLength = 10;
    const int pointsPerLine = 100;
    int rowsDeleted = mBlocksToDelete.size() / rowLength;
    for (const auto& block : mBlocksToDelete)
    {
        gCoordinator.DestroyEntity(block);
    }
    mBlocksToDelete.clear();
    mCanMoveDown = true;

    // Updating the score and lines
    if (rowsDeleted > 0)
    {

        // SDL_Log("Updating Score");
        auto scoreSystem = gCoordinator.GetSystem<ScoreSystem>();
        scoreSystem->score += pointsPerLine * rowsDeleted * rowsDeleted * scoreSystem->level;
        scoreSystem->lines += rowsDeleted;
        scoreSystem->level = (scoreSystem->lines / 10) + 1;

        scoreSystem->Update();
    }
}

void RowSystem::Close()
{
    std::set<Entity> copyEntities = mEntities;
    for (const auto& entity : copyEntities)
    {
        gCoordinator.GetComponent<TextureComponent>(entity).destroy();
        gCoordinator.DestroyEntity(entity);
    }
}
