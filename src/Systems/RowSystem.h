#pragma once

#include "../Core/ECS.h"
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"

class RowSystem : public System
{
public:
    void Update();
};
