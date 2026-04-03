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

    for (size_t y_start = 0; y_start < SIZE_Y; y_start++)
    {
        __m256 c_y_v = _mm256_set_ps(DUP_8(resolution * ((float) CENTER_Y - (float) y_start)));

        for (size_t x_start = 0; x_start < SIZE_X / FLOAT_CAPACITY; x_start++)
        {
            __m256 c_x_v = _mm256_set_ps(DUP_8(resolution *
                        ((float) x_start * FLOAT_CAPACITY - (float) CENTER_X)));
            c_x_v = _mm256_add_ps(c_x_v, step_v);
            size_t run_number = 0;
            
            __m256 x_cur_v = c_x_v;
            __m256 y_cur_v = c_y_v;
            __m256i run_v = _mm256_setzero_si256();

            while (run_number < MAX_RUN_NUMBER)
            {
/*x^2 =*/       __m256 x_sqr_v = _mm256_mul_ps(x_cur_v, x_cur_v);
/*y^2 =*/       __m256 y_sqr_v = _mm256_mul_ps(y_cur_v, y_cur_v);
/*x*y =*/       __m256 x_mul_y_v = _mm256_mul_ps(x_cur_v, y_cur_v); 

// x_next = x_prev * x_prev - y_prev * y_prev + c_x;
                x_cur_v = _mm256_add_ps(_mm256_sub_ps(x_sqr_v, y_sqr_v), c_x_v);
// y_next = 2 * x_prev * y_prev + c_y;
                y_cur_v = _mm256_add_ps(_mm256_add_ps(x_mul_y_v, x_mul_y_v), c_y_v);

                __m256 sqr_sum = _mm256_add_ps(x_sqr_v, y_sqr_v);
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

            __m256i* address = (__m256i*) (pixels + PIX_LEN * (FLOAT_CAPACITY * x_start 
                                + SIZE_X * y_start));
            _mm256_storeu_si256(address , run_v);               
        }
    }
}

#undef DUP_8
