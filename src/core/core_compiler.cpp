#include "core.h"

#include <assert.h>
#include <cstdlib>
#include <cstring>

#include "visual.h"

static const size_t CENTER_X = SIZE_X / 2;
static const size_t CENTER_Y = SIZE_Y / 2;
static const size_t FLOAT_CAPACITY = 16;

///////////////////////////// inline functions ////////////////////////////////

static const size_t VECTOR_SIZE = FLOAT_CAPACITY;

typedef union 
{
    float vec[VECTOR_SIZE]; 
    int vec_i[VECTOR_SIZE]; 
} __mm;

#define CYCLE for(size_t i = 0; i < VECTOR_SIZE; i++) 

inline __mm _mm_set_ps(float value) {   __mm ret = {}; CYCLE {ret.vec[i] = value; } return ret; }
inline __mm _mm_set_epi32(int value) { __mm ret = {}; CYCLE {ret.vec_i[i] = value; } return ret;  }

inline __mm _mm_mul_ps(__mm a, __mm b) { __mm ret = {}; CYCLE { ret.vec[i] = a.vec[i] * b.vec[i]; } return ret; };
inline __mm _mm_mul_epi32(__mm a, __mm b) { __mm ret = {}; CYCLE { ret.vec_i[i] = a.vec_i[i] * b.vec_i[i]; } return ret; };

inline __mm _mm_add_ps(__mm a, __mm b) { __mm ret = {}; CYCLE { ret.vec[i] = a.vec[i] + b.vec[i]; } return ret; };
inline __mm _mm_add_epi32(__mm a, __mm b) { __mm ret = {}; CYCLE { ret.vec_i[i] = a.vec_i[i] + b.vec_i[i]; } return ret; };

inline __mm _mm_sub_ps(__mm a, __mm b) { __mm ret = {}; CYCLE { ret.vec[i] = a.vec[i] - b.vec[i]; } return ret; };
inline __mm _mm_sub_epi32(__mm a, __mm b) { __mm ret = {}; CYCLE { ret.vec_i[i] = a.vec_i[i] - b.vec_i[i]; } return ret; };

inline __mm _mm_setzero() { __mm ret = {.vec = {}};  return ret; };

inline __mm _mm_cmp_ps_ge(__mm a, __mm b) { __mm ret = {}; 
                                                CYCLE { ret.vec_i[i] = a.vec[i] > b.vec[i] ? -1 : 0;} return ret; }; // -1 == 0xffffffff
inline int _mm_testz_ps(__mm a, __mm b) { CYCLE { if (a.vec_i[i] && b.vec_i[i]) return 0;} return 1; }; 

inline __mm _mm_and(__mm a, __mm b) { __mm ret = {}; CYCLE { ret.vec_i[i] = a.vec_i[i] & b.vec_i[i]; } return ret; }; 
inline __mm _mm_or(__mm a, __mm b) { __mm ret = {}; CYCLE { ret.vec_i[i] = a.vec_i[i] | b.vec_i[i]; } return ret; }; 
inline __mm _mm_slli_epi32(__mm a, int c) { __mm ret = {}; CYCLE { ret.vec_i[i] = a.vec_i[i] << c; } return ret; }; 

inline void _mm_store(void* dst , __mm a) { int* int_dst = (int*) dst; CYCLE { *int_dst = a.vec_i[i]; int_dst++;}};

#undef CYCLE 

///////////////////////////////////////////////////////////////////////////////

void 
CalculateMandelbrot(core_t core)
{
    assert(core != nullptr);

    pixel_t* pixels = core->pixels;
    float resolution = core->resolution;

    __mm ladder = {}; for (size_t i = 0; i < VECTOR_SIZE; i++) ladder.vec[i] = (float) i;

    __mm resolution_v = _mm_set_ps(resolution);
    __mm step_v = _mm_mul_ps(ladder, resolution_v);

    __mm max_dest_v = _mm_set_ps(MAX_DISTANCE);
    __mm one_v = _mm_set_epi32(1);

        for (size_t scr_y = 0; scr_y < SIZE_Y; scr_y++)
    {
        __mm c_y_v_0 = _mm_set_ps(resolution * ((float) CENTER_Y - (float) scr_y) + core->center_y);

        for (size_t scr_x = 0; scr_x < SIZE_X / FLOAT_CAPACITY; scr_x++)
        {
            __mm c_x_v_0 = _mm_set_ps(resolution * ((float) scr_x * FLOAT_CAPACITY 
                                            - (float) CENTER_X) - core->center_x);
            c_x_v_0 = _mm_add_ps(c_x_v_0, step_v);
            size_t run_number = 0;
            
            __mm c_x_v = c_x_v_0;
            __mm c_y_v = c_y_v_0;
            __mm run_v = _mm_setzero();

            while (run_number < MAX_RUN_NUMBER)
            {
/*x^2 =*/       __mm c_x_sqr_v = _mm_mul_ps(c_x_v, c_x_v);
/*y^2 =*/       __mm c_y_sqr_v = _mm_mul_ps(c_y_v, c_y_v);
/*x*y =*/       __mm c_x_mul_y_v = _mm_mul_ps(c_y_v, c_x_v); 

// x_next = x_prev * x_prev - y_prev * y_prev + c_x;
                c_x_v = _mm_add_ps(_mm_sub_ps(c_x_sqr_v, c_y_sqr_v), c_x_v_0);
// y_next = 2 * x_prev * y_prev + c_y;
                c_y_v = _mm_add_ps(_mm_add_ps(c_x_mul_y_v, c_x_mul_y_v), c_y_v_0);

                __mm sqr_sum = _mm_add_ps(c_x_sqr_v, c_y_sqr_v);
                __mm is_greater = _mm_cmp_ps_ge(max_dest_v, sqr_sum);
                int is_end = _mm_testz_ps(is_greater, is_greater);
                if (is_end)
                {
                    break;
                }    

                run_v = _mm_add_epi32(run_v, _mm_and(is_greater, one_v));

                run_number++;
            }

            __mm color_mask = _mm_set_epi32(0x00'ff'ff'ff);
            run_v = _mm_slli_epi32(run_v,  24);
            run_v = _mm_or(color_mask, run_v);

            void* address = (pixels + PIX_LEN * (FLOAT_CAPACITY * scr_x + SIZE_X * scr_y));
            _mm_store(address , run_v);               
        }
    }
}

