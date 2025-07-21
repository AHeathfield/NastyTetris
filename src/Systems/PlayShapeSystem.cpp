#include "PlayShapeSystem.h"

void PlayShapeSystem::PlayNextShape(Shape* nextShape)
{
    // Need to like make the ShapeSystem update the array by moving all the components up by 1 index, use a bool or something
    nextShape->MakeMovable();
    currentShape = nextShape;
}

bool PlayShapeSystem::isNoPlayingShape()
{
    return mEntities.empty();
}
