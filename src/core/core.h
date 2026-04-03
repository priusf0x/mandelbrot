#ifndef CORE_H
#define CORE_H

#include <stdint.h>
#include <stdlib.h>

typedef uint8_t pixel_t; 
struct core_s
{
    pixel_t* pixels;
    float    resolution;
    float    center_x;
    float    center_y;
};
typedef core_s* core_t;


const size_t MAX_RUN_NUMBER = 255;
const float MAX_DISTANCE = 3.0f;
const size_t PIX_LEN = 4;

void 
CalculateMandelbrot(core_t core);

#endif // CORE_H
