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
    __m256 offset_v = _mm256_set_ps(DUP_8(8 * resolution)); 

    __m256 max_dest_v = _mm256_set_ps(DUP_8(MAX_DISTANCE));
    __m256i one_v = _mm256_set_epi32(DUP_8(1));

    for (size_t scr_y = 0; scr_y < SIZE_Y; scr_y++)
    {
        __m256 c_y_v_start = _mm256_set_ps(DUP_8(resolution * ((float) CENTER_Y - (float) scr_y) + core->center_y));

        for (size_t scr_x = 0; scr_x < (SIZE_X / FLOAT_CAPACITY) / 3; scr_x++)
        {
            __m256 c_x_v_1_start = _mm256_set_ps(DUP_8(resolution * 
                                    ((float) scr_x * FLOAT_CAPACITY * 3 - (float) CENTER_X) - core->center_x));
            c_x_v_1_start = _mm256_add_ps(c_x_v_1_start, step_v);
            __m256 c_x_v_2_start = _mm256_add_ps(c_x_v_1_start, offset_v);
            __m256 c_x_v_3_start = _mm256_add_ps(c_x_v_2_start, offset_v);
            size_t run_number = 0;
            
            __m256 c_x_v_1 = c_x_v_1_start;
            __m256 c_x_v_2 = c_x_v_2_start;
            __m256 c_x_v_3 = c_x_v_3_start;
            __m256 c_y_v_1 = c_y_v_start;
            __m256 c_y_v_2 = c_y_v_start;
            __m256 c_y_v_3 = c_y_v_start;
            __m256i run_v_1 = _mm256_setzero_si256(), run_v_2 = run_v_1, run_v_3 = run_v_2;

            while (run_number < MAX_RUN_NUMBER)
            {
                __m256 x_sqr_v_1 = _mm256_mul_ps(c_x_v_1, c_x_v_1);
                __m256 x_sqr_v_2 = _mm256_mul_ps(c_x_v_2, c_x_v_2);
                __m256 x_sqr_v_3 = _mm256_mul_ps(c_x_v_3, c_x_v_3);
                __m256 y_sqr_v_1 = _mm256_mul_ps(c_y_v_1, c_y_v_1);
                __m256 y_sqr_v_2 = _mm256_mul_ps(c_y_v_2, c_y_v_2);
                __m256 y_sqr_v_3 = _mm256_mul_ps(c_y_v_3, c_y_v_3);
                __m256 x_mul_y_v_1 = _mm256_mul_ps(c_x_v_1, c_y_v_1); 
                __m256 x_mul_y_v_2 = _mm256_mul_ps(c_x_v_2, c_y_v_2); 
                __m256 x_mul_y_v_3 = _mm256_mul_ps(c_x_v_3, c_y_v_3); 

                c_x_v_1 = _mm256_add_ps(_mm256_sub_ps(x_sqr_v_1, y_sqr_v_1), c_x_v_1_start);
                c_x_v_2 = _mm256_add_ps(_mm256_sub_ps(x_sqr_v_2, y_sqr_v_2), c_x_v_2_start);
                c_x_v_3 = _mm256_add_ps(_mm256_sub_ps(x_sqr_v_3, y_sqr_v_3), c_x_v_3_start);
                c_y_v_1 = _mm256_add_ps(_mm256_add_ps(x_mul_y_v_1, x_mul_y_v_1), c_y_v_start);
                c_y_v_2 = _mm256_add_ps(_mm256_add_ps(x_mul_y_v_2, x_mul_y_v_2), c_y_v_start);
                c_y_v_3 = _mm256_add_ps(_mm256_add_ps(x_mul_y_v_3, x_mul_y_v_3), c_y_v_start);

                __m256 sqr_sum_1 = _mm256_add_ps(x_sqr_v_1, y_sqr_v_1);
                __m256 sqr_sum_2 = _mm256_add_ps(x_sqr_v_2, y_sqr_v_2);
                __m256 sqr_sum_3 = _mm256_add_ps(x_sqr_v_3, y_sqr_v_3);
                __m256 is_greater_1 = _mm256_cmp_ps(sqr_sum_1, max_dest_v, _CMP_LE_OQ);
                __m256 is_greater_2 = _mm256_cmp_ps(sqr_sum_2, max_dest_v, _CMP_LE_OQ);
                __m256 is_greater_3 = _mm256_cmp_ps(sqr_sum_3, max_dest_v, _CMP_LE_OQ);

                int is_end_1 = _mm256_testz_ps(is_greater_1, is_greater_1);
                int is_end_2 = _mm256_testz_ps(is_greater_2, is_greater_2);
                int is_end_3 = _mm256_testz_ps(is_greater_3, is_greater_3);
                if (is_end_1 && is_end_2 && is_end_3)
                {
                    break;
                }    
                run_v_1 = _mm256_add_epi32(run_v_1, _mm256_and_si256((__m256i) is_greater_1, one_v));
                run_v_2 = _mm256_add_epi32(run_v_2, _mm256_and_si256((__m256i) is_greater_2, one_v));
                run_v_3 = _mm256_add_epi32(run_v_3, _mm256_and_si256((__m256i) is_greater_3, one_v));

                run_number++;
            }

            __m256i meow = _mm256_set_epi32(DUP_8(0x00'ff'ff'ff));
            run_v_1 = _mm256_slli_epi32(run_v_1,  24);
            run_v_2 = _mm256_slli_epi32(run_v_2,  24);
            run_v_3 = _mm256_slli_epi32(run_v_3,  24);
            run_v_1 = _mm256_or_si256(meow, run_v_1);
            run_v_2 = _mm256_or_si256(meow, run_v_2);
            run_v_3 = _mm256_or_si256(meow, run_v_3);

            __m256i* address_1 = (__m256i*) (pixels + PIX_LEN * (FLOAT_CAPACITY * scr_x * 3
                                + SIZE_X * scr_y));
            __m256i* address_2 = (__m256i*) (pixels + FLOAT_CAPACITY * PIX_LEN + PIX_LEN * (FLOAT_CAPACITY * scr_x * 3 
                                + SIZE_X * scr_y));
            __m256i* address_3 = (__m256i*) (pixels + FLOAT_CAPACITY * PIX_LEN * 2 + PIX_LEN * (FLOAT_CAPACITY * scr_x * 3 
                                + SIZE_X * scr_y));

            _mm256_storeu_si256(address_1 , run_v_1);               
            _mm256_storeu_si256(address_2 , run_v_2);               
            _mm256_storeu_si256(address_3 , run_v_3);               
        }
    }
}

#undef DUP_8
