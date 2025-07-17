#pragma once

#include "../Core/ECS.h"
#include "../Components/MoveComponent.h"
#include "../Shapes/Shape.h"
#include "../Shapes/LShape.h"
#include "../Shapes/ReverseLShape.h"
#include "../Shapes/TShape.h"
#include "../Shapes/ZShape.h"
#include "../Shapes/ReverseZShape.h"
#include "../Shapes/LineShape.h"
#include "../Shapes/SquareShape.h"

class ShapeSystem : public System
{
public:
    bool Update();

    Shape* currentShape = nullptr;
};
