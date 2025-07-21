#pragma once

#include "../Core/ECS.h"
#include "../Shapes/Shape.h"

class PlayShapeSystem : public System
{
public:
    void PlayNextShape(Shape* nextShape);
    bool isNoPlayingShape();

    Shape* currentShape = nullptr;
};
