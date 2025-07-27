#pragma once

#include "../Core/ECS.h"
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"

class RowSystem : public System
{
public:
    void Update();
};
