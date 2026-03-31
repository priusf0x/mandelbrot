#include "visual.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <assert.h>
#include <stdlib.h>
                 
using namespace std;

///////////////////////////////// screen params ///////////////////////////////

static const char* WINDOW_LABEL = "Mandelvrot";

///////////////////////////////////// Ctor ////////////////////////////////////

visual_return_e
VisualCtor(visual_t* context)
{
    assert(context != nullptr);

    *context = (visual_t) calloc(1, sizeof(visual_s));

    if (*context == nullptr)
    {
        return VISUAL_RETURN_BAD_ALLOC;
    }

    visual_t visual = *context;

    visual->pixels = (pixel_t*) calloc(PIXELS_SIZE, sizeof(pixel_t));

    if (visual->pixels == nullptr)
    {
        free(*context);

        return VISUAL_RETURN_BAD_ALLOC;
    }

    visual->window = new RenderWindow(VideoMode({SIZE_X, SIZE_Y}), WINDOW_LABEL);
    visual->texture = new Texture({SIZE_X, SIZE_Y});
    visual->screen = new Sprite(*visual->texture);
    visual->texture->update(visual->pixels);

    return VISUAL_RETURN_SUCCESS;
}

///////////////////////////////////// Dtor ////////////////////////////////////

visual_return_e
VisualDtor(visual_t* context)
{
    if ((context != nullptr) && (*context != nullptr))
    {
        free((*context)->pixels);
        delete (*context)->texture;
        delete (*context)->screen;
        delete (*context)->window;
        free(*context);

        *context = nullptr;
    }

    return VISUAL_RETURN_SUCCESS;
}

//////////////////////////////// Update Screen ////////////////////////////////

visual_return_e 
UpdateScreen(visual_t context)
{
    assert(context != nullptr);
    
    context->texture->update(context->pixels);
    context->window->draw(*context->screen); 
    context->window->display();

    return VISUAL_RETURN_SUCCESS;
}
