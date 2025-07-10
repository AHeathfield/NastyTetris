#include "MouseButtonSystem.h"
#include <SDL3/SDL_events.h>

extern Coordinator gCoordinator;


void MouseButtonSystem::HandleEvent(SDL_Event* e)
{
    //If mouse event happened
    if( e->type == SDL_EVENT_MOUSE_MOTION || e->type == SDL_EVENT_MOUSE_BUTTON_DOWN || e->type == SDL_EVENT_MOUSE_BUTTON_UP )
    {
        //Get mouse position
        float x = -1.f, y = -1.f;
        SDL_GetMouseState( &x, &y );
        Vector2 mousePos = Vector2(x, y);

        //Check if mouse is in button
        for (const auto& entity : mEntities)
        {
            const auto& texture = gCoordinator.GetComponent<TextureComponent>(entity);
            const auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);
            auto& button = gCoordinator.GetComponent<ButtonComponent*>(entity);

            if (isMouseInside(mousePos, transform.position, texture))
            {
                // activateHoverColor(texture);
                
                if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                {
                    // activateClickColor(texture);
                }
                else if (e->type == SDL_EVENT_MOUSE_BUTTON_UP)
                {
                    button->Click();
                }
            }
        }
    }
}

bool MouseButtonSystem::isMouseInside(Vector2 mousePos, Vector2 buttonPos, const TextureComponent& texture)
{
    //Mouse is left of the button
    if( mousePos.x < buttonPos.x )
    {
        return false;
    }
    //Mouse is right of the button
    else if( mousePos.x > buttonPos.x + texture.width)
    {
        return false;
    }
    //Mouse above the button
    else if( mousePos.y < buttonPos.y )
    {
        return false;
    }
    //Mouse below the button
    else if( mousePos.y > buttonPos.y + texture.height)
    {
        return false;
    }

    return true;
}
