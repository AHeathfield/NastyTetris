#pragma once

#include "../Core/ECS.h"
#include <SDL3/SDL_events.h>

#include "../Components/BoxColliderComponent.h"
#include "../Components/MoveComponent.h"
#include "../Components/HoldComponent.h"
#include "../Shapes/Shape.h"
#include "../Systems/HoldSystem.h"
#include "../Systems/PlayShapeSystem.h"
#include "../Systems/ScoreSystem.h"

class PlayerEventSystem : public System
{
public:
    void HandleEvent(SDL_Event e);
};
