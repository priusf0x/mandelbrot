#ifndef CORE_H
#define CORE_H

#include <stdint.h>
#include <stdlib.h>

typedef uint8_t pixel_t; 
struct core_s
{
    pixel_t* pixels;
    double   resolution;
    double   x_0;
    double   y_0;
};
typedef core_s* core_t;


const size_t MAX_RUN_NUMBER = 255;
const double MAX_DISTANCE = 100.0f;

void 
CalculateMandelbrot(core_t core);

#endif // CORE_H
