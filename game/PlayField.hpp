#pragma once

#include "Settings.hpp"
#include "ChartManager.hpp"
#include "InputManager.hpp"


namespace ne {

class PlayField : public sf::Transformable, public sf::Drawable {
private:
    sf::Clock m_clock;
    sf::Vector2f m_size;

    Settings& m_settings;
    ChartManager& m_chartManager;
    InputManager& m_inputManager;
public:
    PlayField(Settings& settings, ChartManager& chartManager, InputManager& inputManager);

    void setSize(sf::Vector2f size);
    [[nodiscard]] sf::Vector2f getSize() const;

    [[nodiscard]] sf::FloatRect getLocalBounds() const;
    [[nodiscard]] sf::FloatRect getGlobalBounds() const;
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

} //namespace ne
