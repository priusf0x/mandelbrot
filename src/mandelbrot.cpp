#include "mandelbrot.h"

#include <SFML/Main.hpp>
#include <SFML/Window/Context.hpp>

#include "visual.h"
#include "core.h"

//////////////////////////////// Main Cycle ///////////////////////////////////

static const float COORD_INCREMENT = 10;
static const float RES_INCREMENT = 1.10f;

void
ShowMandelbrot(visual_t visual,
               core_t   core)
{
    assert(visual != nullptr);

    while (visual->window->isOpen())
    {
        while (const std::optional<sf::Event> event = visual->window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                visual->window->close();
            }
            else if (event->is<sf::Event::KeyPressed>())
            {
                const auto* key = event->getIf<sf::Event::KeyPressed>();
                const auto scancode = key->scancode;
                switch (scancode)
                {
                        case sf::Keyboard::Scan::W:                        
                            core->center_y += core->resolution * COORD_INCREMENT;
                            break;

                        case sf::Keyboard::Scan::S:                        
                            core->center_y -= core->resolution * COORD_INCREMENT;
                            break;

                        case sf::Keyboard::Scan::A:                        
                            core->center_x += core->resolution * COORD_INCREMENT;
                            break;

                        case sf::Keyboard::Scan::D:                        
                            core->center_x -= core->resolution * COORD_INCREMENT;
                            break;

                        case sf::Keyboard::Scan::Up:                        
                            core->resolution /= RES_INCREMENT;
                            break;

                        case sf::Keyboard::Scan::Down:                        
                            core->resolution *= RES_INCREMENT;
                            break;

                        default: break;
                }
            }
        }
        
        visual->window->clear();
        ShowFPS(visual);
        CalculateMandelbrot(core);
        UpdateScreen(visual);
    }
}
