#pragma once

#include "../Core/ECS.h"
#include <SDL3/SDL_events.h>

#include "../Components/BoxColliderComponent.h"
#include "../Components/MoveComponent.h"

class PlayerEventSystem : public System
{
public:
    void HandleEvent(SDL_Event e);
};
