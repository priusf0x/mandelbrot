#include "core.h"

#include <CL/cl_platform.h>
#include <assert.h>
// #ifdef GPU 
    #include <CL/cl.h>
// #endif //GPU

#include "buffer.h"
#include "visual.h"

static const float DEFAULT_RESOLUTION = 1.0/300;
static const char* const COMP_NAME = "src/core/openCL/kernel.cl";

// smart git

core_ret 
CoreCtor(core_t   core,
         pixel_t* pixel)
{
    assert(core != nullptr);
    assert(pixel != nullptr);

    core->pixels = pixel;
    core->resolution = DEFAULT_RESOLUTION;

#ifdef GPU     

// ПО ХОРОШЕМУ НАДО ХЭНДЛИТЬ ОШИБКИ, НО Я СКИПНУ ЭТОТ
// МОМЕНТ ДЛЯ ЭКОНОМИИ РЕСУРСОВ(время в частности)

    cl_int error = 0;

    cl_platform_id platform = nullptr;
    cl_device_id   device   = nullptr;

    const cl_uint PLATFORM_AMOUNT = 1;
    const cl_uint DEVICE_AMOUNT = 1;
    error = clGetPlatformIDs(PLATFORM_AMOUNT, &platform, nullptr);
    error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, DEVICE_AMOUNT, 
                            &device, nullptr);
    cl_context cl_context = clCreateContext(NULL, DEVICE_AMOUNT, &device,
                                                nullptr, nullptr, &error);
    buffer_t buffer = nullptr;
    BufferCtor(&buffer, COMP_NAME);

    const char* code = buffer->buffer;
    const size_t code_len = buffer->buffer_size;

// еще тут не стоит компилить прогу каждый раз, но... (см предложение выше)
    cl_program program = clCreateProgramWithSource(cl_context, 1, 
                                    &code, &code_len, nullptr);

    error = clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);
    cl_kernel kernel = clCreateKernel(program, "CalculateLocalMandelbrot", &error);
    cl_command_queue queue = clCreateCommandQueue(cl_context, device, 0, &error);
    cl_mem buff = clCreateBuffer(cl_context, CL_MEM_WRITE_ONLY, PIXELS_SIZE,
                                    nullptr, &error); 

    core->kernel = kernel;
    core->queue = queue;
    core->gpu_buffer = buff;
    core->context = cl_context;
    core->program = program;
    core->code_buffer = buffer;

#endif // GPU

    return CORE_RET_SUCCESS;
}

core_ret 
CoreDtor(core_t core)
{
    if (core != nullptr)
    {
#ifdef GPU     
            clReleaseKernel(core->kernel);   
            clReleaseMemObject(core->gpu_buffer);   
            clReleaseCommandQueue(core->queue);   
            clReleaseProgram(core->program);   
            clReleaseContext(core->context);   
            BufferDtor(&core->code_buffer);

#endif // GPU
    }

    return CORE_RET_SUCCESS;
}

//  Отладочный код 
// size_t l_size = 0;
// clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &l_size);
//
// char meow [10000] ={};
// clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, l_size, meow, NULL);
//
// printf("%s", meow);


