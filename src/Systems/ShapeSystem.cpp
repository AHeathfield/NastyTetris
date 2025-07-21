#include "ShapeSystem.h"
#include <SDL3/SDL_log.h>
#include <cstdlib>


bool ShapeSystem::Update()
{
    bool isNewShape = false;
    int shapeNum = 0;
    while (mHoldingShapes.size() < 3)
    {
        isNewShape = true;
        shapeNum = rand() % 7;

        switch (shapeNum)
        {
            case 0:
                mHoldingShapes.push_back(new LShape());
                break;
            case 1:
                mHoldingShapes.push_back(new ReverseLShape());
                break;
            case 2:
                mHoldingShapes.push_back(new TShape());
                break;
            case 3:
                mHoldingShapes.push_back(new ZShape());
                break;
            case 4:
                mHoldingShapes.push_back(new ReverseZShape());
                break;
            case 5:
                mHoldingShapes.push_back(new LineShape());
                break;
            case 6:
                mHoldingShapes.push_back(new SquareShape());
                break;
        }
    }

    if (isNewShape)
    {
        // Rearranging so the newest add is at the bottom
        Rearrange();
        InitShapes();
    }

    return isNewShape;

    // if (mEntities.empty())
    // {
    //     int shapeNum = rand() % 7;
    //
    //     switch (shapeNum)
    //     {
    //         case 0:
    //             currentShape = new LShape();
    //             break;
    //         case 1:
    //             currentShape = new ReverseLShape();
    //             break;
    //         case 2:
    //             currentShape = new TShape();
    //             break;
    //         case 3:
    //             currentShape = new ZShape();
    //             break;
    //         case 4:
    //             currentShape = new ReverseZShape();
    //             break;
    //         case 5:
    //             currentShape = new LineShape();
    //             break;
    //         case 6:
    //             currentShape = new SquareShape();
    //             break;
    //     }
    //
    //     currentShape->Init();
    //     return true;
    // }
    // return false;
}

Shape* ShapeSystem::GetNextShape()
{
    // Put at top of screen
    // if (mHoldingShapes.back() == nullptr)
    // {
    //     SDL_Log("NO SHAPES LOADED!!!");
    //     return nullptr;
    // }
    Shape* nextShape = mHoldingShapes.back();
    mHoldingShapes.pop_back();
    nextShape->Move(-520.f, -80.f);

    return nextShape;
}

