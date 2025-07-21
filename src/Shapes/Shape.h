#pragma once

#include "../Core/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/MoveComponent.h"
#include "../Components/TetrisGravityComponent.h"
#include <vector>

extern Coordinator gCoordinator;

class Shape
{
public:
    virtual void Init() = 0;
    virtual void Rotate() = 0;

    // If you rotate it and then rotate 3 more times your back to rotation before the first rotation
    void Undo()
    {
        for (int i = 0; i < 3; i++)
        {
            IncrementRotation();
            Rotate();
        }
    }

    void IncrementRotation()
    {
        mCurrentRotationDegrees = (mCurrentRotationDegrees + 90) % 360;
    }

    void Move(float x, float y)
    {
        // std::vector<Entity> entities;
        // entities.push_back(mBlock1);
        // entities.push_back(mBlock2);
        // entities.push_back(mBlock3);
        // entities.push_back(mBlock4);
        std::vector<Entity> entities = {mBlock1, mBlock2, mBlock3, mBlock4};

        for (const auto& entity : entities)
        {
            // auto& texture = gCoordinator.GetComponent<TextureComponent>(entity);
            auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);
            auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);

            // Transform will update with collider
            // transform.position.x += x;
            // transform.position.y += y;
            collider.position.x += x;
            collider.position.y += y;
        }
    }

    void MakeMovable()
    {
        std::vector<Entity> entities = {mBlock1, mBlock2, mBlock3, mBlock4};

        for (const auto& entity : entities)
        {
            gCoordinator.AddComponent(
                    entity,
                    MoveComponent{
                        .distance = 40.f
                    });
            gCoordinator.AddComponent(
                    entity,
                    TetrisGravityComponent{
                        .gravity = 40.f
                    });
        }
    }

    int mCurrentRotationDegrees = 0;
    bool isRotated = false;
    bool isAlreadyInit = false;
    Entity mBlock1, mBlock2, mBlock3, mBlock4;
};
