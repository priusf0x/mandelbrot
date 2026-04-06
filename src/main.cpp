#include "mandelbrot.h"

#include "visual.h"
#include "core.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef GPU
// #include <CL/cl.h>
#endif // GPU

int main()
{
    visual_t visual = nullptr;
    core_s core = {}; 
    VisualCtor(&visual);
    CoreCtor(&core, visual->pixels); 
    
    ShowMandelbrot(visual, &core);

    VisualDtor(&visual);
    CoreDtor(&core);

    return 0;
}
