#include "core.h"

#include <assert.h>
#include <stdexcept>

#include "visual.h"

static const size_t CENTER_X = SIZE_X / 2;
static const size_t CENTER_Y = SIZE_Y / 2;

////////////////////////////////// helpers ////////////////////////////////////

/////////////////////////////////// core //////////////////////////////////////

void 
CalculateMandelbrot(core_t core)
{
    assert(core != nullptr);

    pixel_t* pixels = core->pixels;
    double resolution = core->resolution;
    double x_0 = core->x_0, y_0 = core->y_0;

    for (size_t y_start = 0; y_start < SIZE_Y; y_start++)
    {
        double y_old = resolution * ((double) CENTER_Y - (double) y_start) + y_0;

        for (size_t x_start = 0; x_start < SIZE_X; x_start++)
        {
            size_t run_number = 0;
            double x_old = resolution * ((double) x_start - (double) CENTER_X) + x_0;
            double x_next = x_old, y_next = y_old, x_prev = x_old, y_prev = y_old;
            
            while (run_number < MAX_RUN_NUMBER)
            {
                x_next = x_prev * x_prev - y_prev * y_prev + x_old;
                y_next = 2 * x_prev * y_prev + y_old;

                x_prev = x_next;
                y_prev = y_next;

                if ((x_next * x_next + y_next * y_next) > MAX_DISTANCE * MAX_DISTANCE)
                {
                    break; 
                }   

                run_number++;
            }
                                                            
            pixels[4 * x_start + 0 + 4 * SIZE_X * y_start] = 255;                        
            pixels[4 * x_start + 1 + 4 * SIZE_Y * y_start] = 255;                        
            pixels[4 * x_start + 2 + 4 * SIZE_Y * y_start] = 255;                        
            pixels[4 * x_start + 3 + 4 * SIZE_Y * y_start] = (pixel_t) run_number;                        
        }
    }

}
