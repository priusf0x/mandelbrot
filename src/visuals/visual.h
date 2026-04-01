#ifndef VISUAL_H
#define VISUAL_H

#include <stdlib.h>

#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "core.h"

const size_t SIZE_X = 800;
const size_t SIZE_Y = 800;
const size_t PIXELS_SIZE = 4 * SIZE_X * SIZE_Y;

using namespace sf;

struct visual_s
{
    RenderWindow* window;
    pixel_t*      pixels;
    Texture*      texture;
    Sprite*       screen;
    Clock*        clock;
    double        pr_frame;
    Text*         fps_counter;
    const Font*   font;
};
typedef visual_s* visual_t;

enum visual_return_e
{
    VISUAL_RETURN_SUCCESS,
    VISUAL_RETURN_BAD_ALLOC
};

///////////////////////////////// ctor/dtor ///////////////////////////////////

visual_return_e
VisualCtor(visual_t* context);

visual_return_e
VisualDtor(visual_t* context);

visual_return_e 
UpdateScreen(visual_t context);

visual_return_e 
ShowFPS(visual_t context);

#endif // VISUAL_H
