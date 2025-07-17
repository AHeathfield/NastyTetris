#include "ShapeSystem.h"
#include <cstdlib>

bool ShapeSystem::Update()
{
    if (mEntities.empty())
    {
        int shapeNum = rand() % 7;

        switch (shapeNum)
        {
            case 0:
                currentShape = new LShape();
                break;
            case 1:
                currentShape = new ReverseLShape();
                break;
            case 2:
                currentShape = new TShape();
                break;
            case 3:
                currentShape = new ZShape();
                break;
            case 4:
                currentShape = new ReverseZShape();
                break;
            case 5:
                currentShape = new LineShape();
                break;
            case 6:
                currentShape = new SquareShape();
                break;
        }
        currentShape->Init();
        return true;
    }
    return false;
}
