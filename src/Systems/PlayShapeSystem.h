#pragma once

#include "../Core/ECS.h"
#include "../Core/Vector2.h"
#include "../Shapes/Shape.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Systems/HoldSystem.h"
#include <vector>

class PlayShapeSystem : public System
{
public:
    void Update();
    void PlayNextShape(Shape* nextShape);
    bool isNoPlayingShape();
    void UpdateRefEntity();

    Shape* currentShape = nullptr;
    Entity mRefEntity;
    Vector2 mRefEntityPosDiff;
    bool canHoldShape = false;
};
