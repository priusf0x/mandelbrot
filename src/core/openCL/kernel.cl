// все аргументы я постарался именовать как в предыдущих примерах, чтобы было понятно
// поэтому не осуждать за капс пжпж 

// оказалось что стандартные заголовки не работают, будем задаватб параметры тут и через инты 

const int SIZE_X = 800;
const int SIZE_Y = 800;

const int CENTER_X = SIZE_X / 2;
const int CENTER_Y = SIZE_Y / 2; 

const int MAX_RUN_NUMBER = 255;
const int MAX_DISTANCE = 10.f;

const int PIX_LEN = 4;

__kernel void 
CalculateLocalMandelbrot(const float             center_x,
                         const float             center_y,
                         const float             resolution,
                         __global unsigned char* pixels)
{
    const int scr_x = get_global_id(0);             // здесь получим позицию work-item  
    const int scr_y = get_global_id(1);             

    if ((scr_x > SIZE_X) || (scr_y > SIZE_Y) )
    {
        return;  // может возникнуть ситуация, когда позиция выйдет, если размер руппы 
    }
    // те item в этом ворпе, которые не прошли проверку работают дальше в холостую 
    // далее код АБСОЛЮТНО аналогичный simple_core.cpp, за исключениев мб типов, потому что нет size_t сволочи...
    // И еще все возвращается какого то ... в интах, а не в unsigned

    int run_number = 0;
    float c_y_0 = resolution * ((float) CENTER_Y - (float) scr_y) + center_y;
    float c_x_0 = resolution * ((float) scr_x - (float) CENTER_X) - center_x; 
    float c_x = c_x_0, c_y = c_y_0;

    while (run_number < MAX_RUN_NUMBER)
    {
        float c_x_sqr = c_x * c_x,
              c_y_sqr = c_y * c_y,
              c_x_y   = c_x * c_y;

        c_x = c_x_sqr - c_y_sqr + c_x_0;
        c_y = 2 * c_x_y + c_y_0;

        if (c_x_sqr + c_y_sqr > MAX_DISTANCE * MAX_DISTANCE)
        {
            break; 
        }   

        run_number++;
    }

    pixels[PIX_LEN * scr_x + 0 + PIX_LEN * SIZE_X * scr_y] = 255;                        
    pixels[PIX_LEN * scr_x + 1 + PIX_LEN * SIZE_X * scr_y] = 255;                        
    pixels[PIX_LEN * scr_x + 2 + PIX_LEN * SIZE_X * scr_y] = 255;                        
    pixels[PIX_LEN * scr_x + 3 + PIX_LEN * SIZE_X * scr_y] = (unsigned char) run_number;                        
}                                                                         
