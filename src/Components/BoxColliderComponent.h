#pragma once

#include "../Core/Vector2.h"

struct BoxColliderComponent
{
    Vector2 position;
    int w;
    int h;
    bool isBottomCollision = false;
};
