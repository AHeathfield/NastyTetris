#pragma once

#include "../Core/ECS.h"

/*
 * State will contain the entities for that state.
 * - It will need to first initialize all entities
 * - It will also need to delete all entities when switching states
 */
class State
{
public:
    virtual ~State() = default; // So we can delete the reference
    virtual void Enter() = 0;
    virtual void Exit() = 0;
};
