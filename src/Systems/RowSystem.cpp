#include "RowSystem.h"
#include <SDL3/SDL_log.h>
#include <string>
#include <unordered_map>
#include <vector>

extern Coordinator gCoordinator;

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

    // Traversing the grid
    for (auto row = grid.begin(); row != grid.end(); row++)
    {
        if (row->second.size() == rowLength)
        {
            // Delete row
            std::string log = "Delete row: " + std::to_string(row->first);
            SDL_Log(log.c_str());
        }
        else
        {
            // Clear the vector
            row->second.clear();
        }
    }
}
