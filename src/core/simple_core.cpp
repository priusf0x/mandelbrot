#include "core.h"

#include <assert.h>
#include <stdexcept>

#include "visual.h"

static const size_t CENTER_X = SIZE_X / 2;
static const size_t CENTER_Y = SIZE_Y / 2;

/////////////////////////////////// core //////////////////////////////////////

void 
CalculateMandelbrot(core_t core)
{
    assert(core != nullptr);

    pixel_t* pixels = core->pixels;
    float resolution = core->resolution;

    for (size_t y_start = 0; y_start < SIZE_Y; y_start++)
    {
        float c_y = resolution * ((float) CENTER_Y - (float) y_start);

        for (size_t x_start = 0; x_start < SIZE_X; x_start++)
        {
            size_t run_number = 0;
            float c_x = resolution * ((float) x_start - (float) CENTER_X);
            float x_next = c_x, y_next = c_y, x_prev = c_x, y_prev = c_y;
            
            while (run_number < MAX_RUN_NUMBER)
            {
                x_next = x_prev * x_prev - y_prev * y_prev + c_x;
                y_next = 2 * x_prev * y_prev + c_y;

                x_prev = x_next;
                y_prev = y_next;

                if (x_next * x_next + y_next * y_next > MAX_DISTANCE * MAX_DISTANCE)
                {
                    break; 
                }   

                run_number++;
            }
                                                            
            pixels[PIX_LEN * x_start + 0 + PIX_LEN * SIZE_X * y_start] = 255;                        
            pixels[PIX_LEN * x_start + 1 + PIX_LEN * SIZE_Y * y_start] = 255;                        
            pixels[PIX_LEN * x_start + 2 + PIX_LEN * SIZE_Y * y_start] = 255;                        
            pixels[PIX_LEN * x_start + 3 + PIX_LEN * SIZE_Y * y_start] = (pixel_t) run_number;                        
        }
    }
}
