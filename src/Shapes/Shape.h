#pragma once

#include "../Core/ECS.h"
#include "../Core/Vector2.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/MoveComponent.h"
#include "../Components/TetrisGravityComponent.h"
#include "../Components/TextureComponent.h"
#include <SDL3/SDL_log.h>
#include <string>
#include <vector>

extern Coordinator gCoordinator;

class Shape
{
public:
    virtual void Init() = 0;
    virtual void Rotate() = 0;

    std::vector<Entity> GetEntities()
    {
        std::vector<Entity> entities = {mBlock1, mBlock2, mBlock3, mBlock4};
        return entities;
    }

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
        mShapePos = mShapePos + Vector2(x, y);

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

    void SetPosition(const Vector2& newPos)
    {
        // Problem is that the event system will use setPosition,
        // I think if we give it like a new HoldComponent instead and let the HoldSystem do its magic
        Vector2 moveVector = newPos - mShapePos;
        // std::string log = "x: " + std::to_string(newPos.x) + ", y: " + std::to_string(newPos.y);
        // SDL_Log(log.c_str());
        // log = "xShape: " + std::to_string(mShapePos.x) + ", yShape: " + std::to_string(mShapePos.y);
        // SDL_Log(log.c_str());

        Move(moveVector.x, moveVector.y);
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

    void RemoveMovable()
    {
        std::vector<Entity> entities = {mBlock1, mBlock2, mBlock3, mBlock4};

        for (const auto& entity : entities)
        {
            gCoordinator.RemoveComponent<MoveComponent>(entity);
            gCoordinator.RemoveComponent<TetrisGravityComponent>(entity);
        }
    }

    // Destroys the shape
    void Destroy()
    {
        std::vector<Entity> entities = GetEntities();

        for (const auto& entity : entities)
        {
            gCoordinator.GetComponent<TextureComponent>(entity).destroy();
            gCoordinator.DestroyEntity(entity);
        }
    }

    
    int mCurrentRotationDegrees = 0;
    bool isRotated = false;
    bool isAlreadyInit = false;
    Vector2 mShapePos = Vector2();
    Entity mBlock1, mBlock2, mBlock3, mBlock4;
};
