#pragma once

#include <vector>
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
    Shape* GetNextShape();

    // Shape* currentShape = nullptr;

private:
    std::vector<Shape*> mHoldingShapes;

    void InitShapes()
    {
        // index 0 is the bottom of the next list, 2 is the top
        for (int i = 0; i < 3; i++)
        {
            if (!mHoldingShapes[i]->isAlreadyInit)
            {
                mHoldingShapes[i]->mShapePos = Vector2(1360.f, 460.f - (i * 120.f));
                mHoldingShapes[i]->Init();
                // mHoldingShapes[i]->Move(480.f, (320.f - (i * 120.f)));
                mHoldingShapes[i]->isAlreadyInit = true;
            }
            else
            {
                mHoldingShapes[i]->Move(0.f, -120.f);
            }

        }
    }

    void Rearrange()
    {
        Shape* temp = mHoldingShapes[2];

        mHoldingShapes[2] = mHoldingShapes[1];
        mHoldingShapes[1] = mHoldingShapes[0];
        mHoldingShapes[0] = temp;

        temp = nullptr;
    }
};
