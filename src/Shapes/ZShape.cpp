#include "ZShape.h"

extern Coordinator gCoordinator;

void ZShape::Init()
{
    mBlock1 = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBlock1,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{80.f, 0.f, 40.f, 40.f},
                .path = "src/Assets/BlockSpriteSheet.png",
            });
    gCoordinator.AddComponent(
            mBlock1,
            TransformComponent{
                .position = Vector2(920.f, 140.f),
                .rotation = Vector2(),
                .scale = Vector2()
            });
    gCoordinator.AddComponent(
            mBlock1,
            TetrisGravityComponent{
                .gravity = 40.f
            });
    gCoordinator.AddComponent(
            mBlock1,
            BoxColliderComponent{
                .position = Vector2(920.f, 140.f),
                .w = 40,
                .h = 40
            });
    gCoordinator.AddComponent(
            mBlock1,
            MoveComponent{
                .distance = 40.f
            });

    mBlock2 = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBlock2,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{80.f, 0.f, 40.f, 40.f},
                .path = "src/Assets/BlockSpriteSheet.png",
            });
    gCoordinator.AddComponent(
            mBlock2,
            TransformComponent{
                .position = Vector2(960.f, 140.f),
                .rotation = Vector2(),
                .scale = Vector2()
            });
    gCoordinator.AddComponent(
            mBlock2,
            TetrisGravityComponent{
                .gravity = 40.f
            });
    gCoordinator.AddComponent(
            mBlock2,
            BoxColliderComponent{
                .position = Vector2(960.f, 140.f),
                .w = 40,
                .h = 40
            });
    gCoordinator.AddComponent(
            mBlock2,
            MoveComponent{
                .distance = 40.f
            });

    mBlock3 = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBlock3,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{80.f, 0.f, 40.f, 40.f},
                .path = "src/Assets/BlockSpriteSheet.png",
            });
    gCoordinator.AddComponent(
            mBlock3,
            TransformComponent{
                .position = Vector2(960.f, 180.f),
                .rotation = Vector2(),
                .scale = Vector2()
            });
    gCoordinator.AddComponent(
            mBlock3,
            TetrisGravityComponent{
                .gravity = 40.f
            });
    gCoordinator.AddComponent(
            mBlock3,
            BoxColliderComponent{
                .position = Vector2(960.f, 180.f),
                .w = 40,
                .h = 40
            });
    gCoordinator.AddComponent(
            mBlock3,
            MoveComponent{
                .distance = 40.f
            });

    mBlock4 = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBlock4,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{80.f, 0.f, 40.f, 40.f},
                .path = "src/Assets/BlockSpriteSheet.png",
            });
    gCoordinator.AddComponent(
            mBlock4,
            TransformComponent{
                .position = Vector2(1000.f, 180.f),
                .rotation = Vector2(),
                .scale = Vector2()
            });
    gCoordinator.AddComponent(
            mBlock4,
            TetrisGravityComponent{
                .gravity = 40.f
            });
    gCoordinator.AddComponent(
            mBlock4,
            BoxColliderComponent{
                .position = Vector2(1000.f, 180.f),
                .w = 40,
                .h = 40
            });
    gCoordinator.AddComponent(
            mBlock4,
            MoveComponent{
                .distance = 40.f
            });


}


void ZShape::Rotate()
{
    // Setting the auto& colliders
    auto& collider1 = gCoordinator.GetComponent<BoxColliderComponent>(mBlock1);
    auto& collider2 = gCoordinator.GetComponent<BoxColliderComponent>(mBlock2);
    auto& collider3 = gCoordinator.GetComponent<BoxColliderComponent>(mBlock3);
    auto& collider4 = gCoordinator.GetComponent<BoxColliderComponent>(mBlock4);
    switch (mCurrentRotationDegrees)
    {
        case 0:
            collider1.position.x += 40.f;
            collider1.position.y -= 40.f;
            collider3.position.x -= 40.f;
            collider3.position.y -= 40.f;
            collider4.position.x -= 80.f;
            break;

        case 90:
            collider1.position.x += 40.f;
            collider1.position.y += 40.f;
            collider3.position.x += 40.f;
            collider3.position.y -= 40.f;
            collider4.position.y -= 80.f;
            break;

        case 180:
            collider1.position.x -= 40.f;
            collider1.position.y += 40.f;
            collider3.position.x += 40.f;
            collider3.position.y += 40.f;
            collider4.position.x += 80.f;
            break;

        case 270:
            collider1.position.x -= 40.f;
            collider1.position.y -= 40.f;
            collider3.position.x -= 40.f;
            collider3.position.y += 40.f;
            collider4.position.y += 80.f;
            break;
    }
}
