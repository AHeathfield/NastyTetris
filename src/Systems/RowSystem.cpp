#include "RowSystem.h"
#include <SDL3/SDL_log.h>
#include <string>
#include <unordered_map>
#include <vector>

extern Coordinator gCoordinator;

// Helper method
void deleteRow(const std::vector<Entity>& row)
{
    for (const auto& entity : row)
    {
        gCoordinator.DestroyEntity(entity);
    }
}

void RowSystem::Update()
{
    const int rowLength = 10;

    std::unordered_map<float, std::vector<Entity>> grid;

    for (const auto& entity : mEntities)
    {
        const auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);

        // if there is no key with this y yet
        if (grid.find(transform.position.y) == grid.end())
        {
            std::vector<Entity> row = {entity};
            grid.insert({transform.position.y, row});
        }
        else
        {
            grid.at(transform.position.y).push_back(entity);
        }
    }

    // Deleting Rows
    int rowsRemoved = 0;

    std::unordered_map<float, std::vector<Entity>> gridCopy = grid;
    for (auto row = gridCopy.begin(); row != gridCopy.end(); row++)
    {
        if (row->second.size() == rowLength)
        {
            // Delete row
            std::string log = "Delete row: " + std::to_string(row->first);
            SDL_Log(log.c_str());
            deleteRow(row->second);
            grid.erase(row->first);
            rowsRemoved++;
        }
        else
        {
            // Clear the vector
            row->second.clear();
        }
    }

    // Moving all down if row was deleted
    const float blockHeight = 40.f;
    for (auto row = grid.begin(); row != grid.end(); row++)
    {
        for (const auto& block : row->second)
        {
            auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(block);
            auto& transform = gCoordinator.GetComponent<TransformComponent>(block);

            collider.position.y += blockHeight * rowsRemoved;
            transform.position.y += blockHeight * rowsRemoved;
        }
    }
}

