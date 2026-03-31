#include "mandelbrot.h"

#include <SFML/Main.hpp>

#include "visual.h"
#include "core.h"

using namespace std;

//////////////////////////////// Main Cycle ///////////////////////////////////

void
ShowMandelbrot(visual_t visual,
               core_t   core)
{
    assert(visual != nullptr);

    Clock clock {};
    double lastTime = 0.0f, currentTime = 0.0f, fps = 0.0f;

    while (visual->window->isOpen())
    {
        while (const optional event = visual->window->pollEvent())
        {
            if (event->is<Event::Closed>())
                visual->window->close();
        }
        
        visual->window->clear();

            //FIXME FIXME FIXME FIXME FIXME
        currentTime = clock.getElapsedTime().asSeconds();
        fps = 1.0f / (currentTime - lastTime);
        lastTime = currentTime;
        printf("%f\n", fps); // FIXME FIIIIIIX ITTTT TEMPROARY
                             
        CalculateMandelbrot(core);

        UpdateScreen(visual);
    }
}
