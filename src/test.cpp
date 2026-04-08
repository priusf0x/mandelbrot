#include "mandelbrot.h"
#include "core.h"
#include "visual.h"

#include <cstdint>
#include <x86intrin.h>  
#include <stdint.h>        
#include <stdio.h>

volatile pixel_t VISUAL[PIXELS_SIZE] = {};
static const size_t NUM_OF_MEASURES = 10000;

static inline uint64_t rdtsc()
{
 unsigned int hi = 0, lo = 0;                   
 __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
 return ((uint64_t)hi << 32) | lo;
}


int main(int argc, char* argv[])
{

    core_s core = {};

    uint64_t start = 0, end = 0;
    CoreCtor(&core, (pixel_t*) VISUAL);

    uint64_t time[NUM_OF_MEASURES] = {};

    for (size_t i = 0; i < NUM_OF_MEASURES; i++)
    {
        start = rdtsc();
        CalculateMandelbrot(&core);
        end = rdtsc();
        time[i] = end - start;
    }

    const char* file_name = "measure.json";

    if (argc > 1)
    {
        file_name = argv[1];
    }
    FILE* file = fopen(file_name, "w+");

    fprintf(file, "{\n");              
    fprintf(file, "\"time\":\n[\n");              
    const size_t warmup = 10;
    for (size_t i = warmup; i < NUM_OF_MEASURES - 1; i++)
    {
        fprintf(file, "%lu,\n", time[i]);
    }                      
    fprintf(file, "%lu\n", time[NUM_OF_MEASURES - 1]);
    fprintf(file, "]\n");              
    fprintf(file, "}\n");              
    fclose(file);

    return 0;
}
