#pragma once

#include "../Core/ECS.h"
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/RowComponent.h"
#include "RenderSystem.h"
#include "../Core/Timer.h"

class RowSystem : public System
{
public:
    void Update();
    void DeleteRows();
    // void MoveBlocksDown();

private:
    std::unordered_map<float, std::vector<Entity>> mGrid;
    std::vector<Entity> mBlocksToDelete;
    int mRowsRemoved = 0;
    bool mCanMoveDown = false;
};
