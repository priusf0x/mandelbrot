#include "core.h"

#include <algorithm>
#include <assert.h>
#include <cstdint>
#include <cstdlib>
#include <immintrin.h>

#include "visual.h"

static const size_t CENTER_X = SIZE_X / 2;
static const size_t CENTER_Y = SIZE_Y / 2;
static const size_t FLOAT_CAPACITY = 8;

#define DUP_8(DATA_) (DATA_), (DATA_), (DATA_), (DATA_), (DATA_), (DATA_), (DATA_), (DATA_)

void 
CalculateMandelbrot(core_t core)
{
    assert(core != nullptr);

    pixel_t* pixels = core->pixels;
    float resolution = core->resolution;

    __m256 ladder = {0, 1, 2, 3, 4, 5, 6, 7};
    __m256 resolution_v = _mm256_set_ps(DUP_8(resolution));
    __m256 step_v = _mm256_mul_ps(ladder, resolution_v);

    __m256 max_dest_v = _mm256_set_ps(DUP_8(MAX_DISTANCE));
    __m256i one_v = _mm256_set_epi32(DUP_8(1));

    for (size_t scr_y = 0; scr_y < SIZE_Y; scr_y++)
    {
        __m256 c_y_v_0 = _mm256_set_ps(DUP_8(resolution * ((float) CENTER_Y - (float) scr_y) + core->center_y));

        for (size_t scr_x = 0; scr_x < SIZE_X / FLOAT_CAPACITY; scr_x++)
        {
            __m256 c_x_v_0 = _mm256_set_ps(DUP_8(resolution *
                        ((float) scr_x * FLOAT_CAPACITY - (float) CENTER_X) - core->center_x));
            c_x_v_0 = _mm256_add_ps(c_x_v_0, step_v);
            size_t run_number = 0;
            
            __m256 c_x_v = c_x_v_0;
            __m256 c_y_v = c_y_v_0;
            __m256i run_v = _mm256_setzero_si256();

            while (run_number < MAX_RUN_NUMBER)
            {
/*x^2 =*/       __m256 c_x_sqr_v = _mm256_mul_ps(c_x_v, c_x_v);
/*y^2 =*/       __m256 c_y_sqr_v = _mm256_mul_ps(c_y_v, c_y_v);
/*x*y =*/       __m256 c_x_mul_y_v = _mm256_mul_ps(c_x_v, c_y_v); 

// x_next = x_prev * x_prev - y_prev * y_prev + c_x;
                c_x_v = _mm256_add_ps(_mm256_sub_ps(c_x_sqr_v, c_y_sqr_v), c_x_v_0);
// y_next = 2 * x_prev * y_prev + c_y;
                c_y_v = _mm256_add_ps(_mm256_add_ps(c_x_mul_y_v, c_x_mul_y_v), c_y_v_0);

                __m256 sqr_sum = _mm256_add_ps(c_y_sqr_v, c_x_sqr_v);
                __m256 is_greater = _mm256_cmp_ps(sqr_sum, max_dest_v, _CMP_LE_OQ);
                int is_end = _mm256_testz_ps(is_greater, is_greater);
                if (is_end)
                {
                    break;
                }    
                run_v = _mm256_add_epi32(run_v, _mm256_and_si256((__m256i) is_greater, one_v));

                run_number++;
            }

            __m256i meow = _mm256_set_epi32(DUP_8(0x00'ff'ff'ff));
            run_v = _mm256_slli_epi32(run_v,  24);
            run_v = _mm256_or_si256(meow, run_v);

            __m256i* address = (__m256i*) (pixels + PIX_LEN * (FLOAT_CAPACITY * scr_x
                                + SIZE_X * scr_y));
            _mm256_storeu_si256(address , run_v);               
        }
    }
}

#undef DUP_8
