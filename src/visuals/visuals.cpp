#include "core.h"
#include "visual.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <assert.h>
#include <cstdint>
#include <cstdio>
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

    const char* font_name = "assets/JetBrains.ttf";
    visual->font = new Font(font_name);

    const char* init_text = "FPS: 0";
    visual->fps_counter = new Text(*visual->font, init_text);
    visual->window = new RenderWindow(VideoMode({SIZE_X, SIZE_Y}), WINDOW_LABEL);
    visual->texture = new Texture({SIZE_X, SIZE_Y});
    visual->screen = new Sprite(*visual->texture);
    visual->clock = new Clock();
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
        delete (*context)->font;
        delete (*context)->fps_counter;
        delete (*context)->texture;
        delete (*context)->screen;
        delete (*context)->window;
        delete (*context)->clock;
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

//////////////////////////////// FPS counter //////////////////////////////////

visual_return_e 
ShowFPS(visual_t context)
{
    assert(context != nullptr);

    const size_t frame_upd = 100;
    context->window->draw(*context->fps_counter);

    if (context->frame_counter < frame_upd)
    {
        context->frame_counter++;

        return VISUAL_RETURN_SUCCESS;
    }

    context->frame_counter = 0;
    
    const size_t buffer_size = 256;
    static char buffer[buffer_size] = {};

    double cur_frame = (*context->clock).getElapsedTime().asSeconds();
    double fps = 1.0f / (cur_frame - context->pr_frame) * frame_upd;
    context->pr_frame = cur_frame;

    const char* fps_format = "FPS: %.1f";
    snprintf(buffer, buffer_size, fps_format, fps);

    context->fps_counter->setString(buffer);

    return VISUAL_RETURN_SUCCESS;
}



