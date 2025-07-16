#include "ShapeSystem.h"

bool ShapeSystem::Update()
{
    if (mEntities.empty())
    {
        currentShape = new LShape();
        currentShape->Init();
        return true;
    }
    return false;
}
