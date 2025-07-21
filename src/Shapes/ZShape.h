#pragma once

#include "../Core/ECS.h"
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"

#include "Shape.h"

class ZShape : public Shape
{
public:
    void Init() override;
    void Rotate() override;

private:
    // Entity mBlock1, mBlock2, mBlock3, mBlock4;
};
