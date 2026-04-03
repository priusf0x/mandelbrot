#include "mandelbrot.h"

#include <SFML/Main.hpp>

#include "visual.h"
#include "core.h"

//////////////////////////////// Main Cycle ///////////////////////////////////

void
ShowMandelbrot(visual_t visual,
               core_t   core)
{
    assert(visual != nullptr);

    while (visual->window->isOpen())
    {
        while (const std::optional<sf::Event> event = visual->window->pollEvent())
        {
            if (event->is<Event::Closed>())
                visual->window->close();
        }
        
        visual->window->clear();

        ShowFPS(visual);

        CalculateMandelbrot(core);

        UpdateScreen(visual);
    }
}
