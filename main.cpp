#include <iosfwd>

#include "game/ChartManager.hpp"
#include "game/InputManager.hpp"
#include "game/PlayField.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 800}), "Test");
    window.setVerticalSyncEnabled(true);

    ne::Settings settings{};
    ne::ChartManager chartManager("songs/2 - 184/2.qua");
    ne::InputManager inputManager;

    ne::PlayField playField(settings, chartManager, inputManager);
    playField.setSize({300.0f, 800.0f});
    playField.setPosition({400.0f, 400.0f});
    playField.setOrigin(playField.getLocalBounds().getCenter());

    while(window.isOpen())
    {
        while(const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear(sf::Color::Blue);
        window.draw(playField);
        window.display();
    }
}