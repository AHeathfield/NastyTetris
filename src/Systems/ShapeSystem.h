#pragma once

#include "../Core/ECS.h"
#include "../Components/MoveComponent.h"
#include "../Shapes/Shape.h"
#include "../Shapes/LShape.h"

class ShapeSystem : public System
{
public:
    bool Update();

    Shape* currentShape = nullptr;
};
