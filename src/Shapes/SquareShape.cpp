#include "SquareShape.h"

extern Coordinator gCoordinator;

void SquareShape::Init()
{
    mBlock1 = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBlock1,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 40.f, 40.f},
                .path = "src/Assets/BlockSpriteSheet.png",
            });
    gCoordinator.AddComponent(
            mBlock1,
            TransformComponent{
                .position = mShapePos + Vector2(0.f, 0.f),
                .rotation = Vector2(),
                .scale = Vector2()
            });
    gCoordinator.AddComponent(
            mBlock1,
            BoxColliderComponent{
                .position = mShapePos + Vector2(0.f, 0.f),
                .w = 40,
                .h = 40
            });

    mBlock2 = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBlock2,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 40.f, 40.f},
                .path = "src/Assets/BlockSpriteSheet.png",
            });
    gCoordinator.AddComponent(
            mBlock2,
            TransformComponent{
                .position = mShapePos + Vector2(40.f, 0.f),
                .rotation = Vector2(),
                .scale = Vector2()
            });
    gCoordinator.AddComponent(
            mBlock2,
            BoxColliderComponent{
                .position = mShapePos + Vector2(40.f, 0.f),
                .w = 40,
                .h = 40
            });

    mBlock3 = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBlock3,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 40.f, 40.f},
                .path = "src/Assets/BlockSpriteSheet.png",
            });
    gCoordinator.AddComponent(
            mBlock3,
            TransformComponent{
                .position = mShapePos + Vector2(0.f, 40.f),
                .rotation = Vector2(),
                .scale = Vector2()
            });
    gCoordinator.AddComponent(
            mBlock3,
            BoxColliderComponent{
                .position = mShapePos + Vector2(0.f, 40.f),
                .w = 40,
                .h = 40
            });

    mBlock4 = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBlock4,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 40.f, 40.f},
                .path = "src/Assets/BlockSpriteSheet.png",
            });
    gCoordinator.AddComponent(
            mBlock4,
            TransformComponent{
                .position = mShapePos + Vector2(40.f, 40.f),
                .rotation = Vector2(),
                .scale = Vector2()
            });
    gCoordinator.AddComponent(
            mBlock4,
            BoxColliderComponent{
                .position = mShapePos + Vector2(40.f, 40.f),
                .w = 40,
                .h = 40
            });


}


void SquareShape::Rotate()
{
    // Setting the auto& colliders
    auto& collider1 = gCoordinator.GetComponent<BoxColliderComponent>(mBlock1);
    auto& collider2 = gCoordinator.GetComponent<BoxColliderComponent>(mBlock2);
    auto& collider3 = gCoordinator.GetComponent<BoxColliderComponent>(mBlock3);
    auto& collider4 = gCoordinator.GetComponent<BoxColliderComponent>(mBlock4);
    switch (mCurrentRotationDegrees)
    {
        case 0:
            break;

        case 90:
            break;

        case 180:
            break;

        case 270:
            break;
    }
}
