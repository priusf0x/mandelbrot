#include "core.h"

#include <assert.h>

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

    for (size_t scr_y = 0; scr_y < SIZE_Y; scr_y++)
    {
        float c_y_0 = resolution * ((float) CENTER_Y - (float) scr_y) + core->center_y;

        for (size_t scr_x = 0; scr_x < SIZE_X; scr_x++)
        {
            size_t run_number = 0;
            float c_x_0 = resolution * ((float) scr_x - (float) CENTER_X) - core->center_y; 
            float c_x = c_x_0, c_y = c_y_0;
            
            while (run_number < MAX_RUN_NUMBER)
            {
                float c_x_sqr = c_x * c_x,
                      c_y_sqr = c_y * c_y,
                      c_x_y   = c_x * c_y;

                c_x = c_x_sqr - c_y_sqr + c_x_0;
                c_y = 2 * c_x_y + c_y_0;

                if (c_x_sqr + c_y_sqr > MAX_DISTANCE * MAX_DISTANCE)
                {
                    break; 
                }   

                run_number++;
            }
                                                            
            pixels[PIX_LEN * scr_x + 0 + PIX_LEN * SIZE_X * scr_y] = 255;                        
            pixels[PIX_LEN * scr_x + 1 + PIX_LEN * SIZE_Y * scr_y] = 255;                        
            pixels[PIX_LEN * scr_x + 2 + PIX_LEN * SIZE_Y * scr_y] = 255;                        
            pixels[PIX_LEN * scr_x + 3 + PIX_LEN * SIZE_Y * scr_y] = (pixel_t) run_number;                        
        }
    }
}
