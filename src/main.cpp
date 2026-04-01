#include "visual.h"
#include "mandelbrot.h"

#include <stdlib.h>

int main()
{
    visual_t visual = nullptr;
    VisualCtor(&visual);

    core_s core = {.pixels = visual->pixels, .resolution = 1.0/300};
    core_t context = &core;

    ShowMandelbrot(visual, context);

    VisualDtor(&visual);

    return 0;
}
