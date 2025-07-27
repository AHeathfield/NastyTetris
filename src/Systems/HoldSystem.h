#pragma once

#include "../Core/ECS.h"
#include "../Shapes/Shape.h"
#include "../Core/Vector2.h"
#include "../Components/HoldComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Systems/PlayShapeSystem.h"

class HoldSystem : public System
{
public:
    void Update();
    void HoldShape(Shape* newShape);
    // Shape* ReleaseShape();
 
    Shape* currentShape = nullptr;
    bool canRelease = false;
    bool canHold = true;
    
    bool spaceWasPressed = false;

private:
    Shape* oldShape = nullptr;

};
