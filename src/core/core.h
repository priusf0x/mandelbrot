#ifndef CORE_H
#define CORE_H

#include <stdint.h>
#include <stdlib.h>

#ifdef GPU
    #include "CL/cl.h"
    #include "buffer.h"
#endif //GPU

typedef uint8_t pixel_t; 
struct core_s
{
    pixel_t*  pixels;
    float     resolution;
    float     center_x;
    float     center_y;
#ifdef GPU
    cl_kernel        kernel;
    cl_command_queue queue;
    cl_mem           gpu_buffer;
    buffer_t         code_buffer;    
    cl_context       context;
    cl_program       program;
#endif // GPU
};
typedef core_s* core_t;


const size_t MAX_RUN_NUMBER = 255;
const float MAX_DISTANCE = 3.0f;
const size_t PIX_LEN = 4;

// ================================= ctor/dtor ================================

enum core_ret
{
    CORE_RET_SUCCESS,
    CORE_RET_GPU_ERR
};

core_ret 
CoreCtor(core_t core, pixel_t* pixel);

core_ret 
CoreDtor(core_t core);

void 
CalculateMandelbrot(core_t core);

#endif // CORE_H
