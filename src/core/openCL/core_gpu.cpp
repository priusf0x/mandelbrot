#include "core.h"
#include <stdio.h>

#include "visual.h"

#include <assert.h>
#include <CL/cl.h>

const size_t LOCAL_SIZE_X = 256;
const size_t LOCAL_SIZE_Y = 4;

void 
CalculateMandelbrot(core_t core)
{
// Передача аргументов 
    clSetKernelArg(core->kernel, 0, sizeof(float), &core->center_x);
    clSetKernelArg(core->kernel, 1, sizeof(float), &core->center_y);
    clSetKernelArg(core->kernel, 2, sizeof(float), &core->resolution);
    clSetKernelArg(core->kernel, 3, sizeof(cl_mem), &core->gpu_buffer);


    size_t local_size[2] = {LOCAL_SIZE_X, LOCAL_SIZE_Y};
    size_t global_size[2] = {(LOCAL_SIZE_X * (SIZE_X / LOCAL_SIZE_X + 1)), 
                             (LOCAL_SIZE_Y * (SIZE_Y / LOCAL_SIZE_Y + 1))};
    
    clEnqueueNDRangeKernel(core->queue, core->kernel, 2, nullptr, 
                            global_size, local_size, 0, nullptr, nullptr);

    clEnqueueReadBuffer(core->queue, core->gpu_buffer, CL_TRUE, 0, 
                            PIXELS_SIZE, core->pixels, 0, nullptr, nullptr);

    clFinish(core->queue);
}
