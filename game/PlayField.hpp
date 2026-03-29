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

PlayField::PlayField(Settings& settings, ChartManager& chartManager, InputManager& inputManager) :
    m_settings(settings),
    m_chartManager(chartManager),
    m_inputManager(inputManager)
{}

void PlayField::setSize(sf::Vector2f size) {
    m_size = size;
}

sf::Vector2f PlayField::getSize() const {
    return m_size;
}

sf::FloatRect PlayField::getLocalBounds() const {
    return sf::FloatRect(sf::Vector2f(0.0f, 0.0f), m_size);
}

sf::FloatRect PlayField::getGlobalBounds() const {
    return getTransform().transformRect(getLocalBounds());
}


/* Too lazy to write right now, check in later!
void PlayField::process() {
    static constexpr sf::Time hitWindow = sf::milliseconds(120);
    static constexpr sf::Time computeBuffer = sf::milliseconds(500);


    sf::Time adjustedTime = m_clock.getElapsedTime();
    sf::Time upperTimeBoundry = currentTime + hitWindow;
    sf::Time lowerTimeBoundry = currentTime - hitWindow;

    auto hitObjects = m_chartManager.query(lowerTimeBoundry - computeBuffer, upperTimeBoundry);

    for(auto& hitObject : hitObjects)
    {       
        if(hitObject.noteHitstate == Hitstate::None)
        {
            if(hitObject.startTime < lowerTimeBoundry)
            {        
                //Missed judgement window
                hitObject.noteHitstate = Hitstate::Failed;
                hitObject.sliderHitstate = Hitstate::Failed;
            }
            else if(m_inputManager.isPressed(hitObject.lane) && hitObject.startTime < upperTimeBoundry)
            {
                //Hit within judgement window
                hitObject.noteHitstate = Hitstate::Completed;
            }
        }

        if(hitObject.noteHitstate == Hitstate::Completed && hitObject.sliderHitstate == Hitstate::None)
        {
            if(!m_inputManager.isHeld(hitObject.lane))
            {
                hitObject.noteHitstate = Hitstate::Failed;   
            }
            else if(currentTime >= hitObject.endTime)
            {
                //Held for sliders full duration (would have failed out eariler)
                hitObject.noteHitstate = Hitstate::Completed;            
            }
        }
    }
}
*/

void PlayField::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform = getTransform();
    
    int keyCount = 4;

    float laneSize = m_size.x / static_cast<float>(keyCount);
    float halfLaneSize = laneSize / 2.0f;

    float judgementY = m_size.y * m_settings.judgmentLine;

    sf::Time adjustedTime = m_clock.getElapsedTime() - sf::milliseconds(judgementY / m_settings.scrollSpeed);    
    sf::Time lowerTimeBoundry = adjustedTime;
    sf::Time upperTimeBoundry = adjustedTime + sf::milliseconds((m_size.y + laneSize) / m_settings.scrollSpeed);
    
    auto hitObjects = m_chartManager.query(lowerTimeBoundry, upperTimeBoundry);
    

    //-------------------------------- TEMPORARY ------------------------------------
    sf::RectangleShape backing(m_size);
    backing.setFillColor(sf::Color::Black); 
    target.draw(backing, states);
    //-------------------------------------------------------------------------------


    //Draw receptors
    sf::CircleShape receptor(halfLaneSize);
    receptor.setFillColor(sf::Color::Yellow);

    for(int i=0; i<keyCount; i++)
    {
        receptor.setPosition({laneSize * i, m_size.y - judgementY});
        target.draw(receptor, states);
    }

    //Draw notes & sliders
    for(const auto& hitObject : hitObjects)
    {
        float startTimeInMs = (hitObject.startTime - adjustedTime).asMilliseconds();
        float endTimeInMs = (hitObject.endTime - adjustedTime).asMilliseconds();
        
        float laneX = laneSize * (hitObject.lane - 1);
        
        float startY = m_size.y - (startTimeInMs * m_settings.scrollSpeed);
        float endY = m_size.y - (endTimeInMs * m_settings.scrollSpeed);
        
        if(hitObject.sliderHitstate != Hitstate::Completed && hitObject.endTime > hitObject.startTime)
        {
            float sliderHeight = std::min(startY + halfLaneSize, m_size.y) - endY;

            sf::RectangleShape slider({laneSize, sliderHeight});
            slider.setPosition({laneX, endY});
            slider.setFillColor(sf::Color::Red);

            target.draw(slider, states);
        }
       
        if(hitObject.noteHitstate == Hitstate::None && hitObject.startTime > lowerTimeBoundry)
        {
            sf::CircleShape note(halfLaneSize);
            note.setPosition({laneX, startY});

            target.draw(note, states);
        }
    }
}


} //namespace ne