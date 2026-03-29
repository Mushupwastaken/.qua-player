#pragma once

#include <yaml-cpp/yaml.h>


namespace ne {

enum class Hitstate {
    None = 0,
    Failed = 1,
    Completed = 2
};

struct HitObject {
    sf::Time startTime;
    sf::Time endTime;
    int lane;

    //For input processing
    Hitstate noteHitstate{};
    Hitstate sliderHitstate{};
};

class ChartManager {
private:
    std::vector<HitObject> m_data;
public:
    ChartManager(const YAML::Node& root);
    ChartManager(const std::string& path);
    
    std::span<HitObject> query(sf::Time startTime, sf::Time endTime);
    std::span<const HitObject> query(sf::Time startTime, sf::Time endTime) const;
};

} //namespace ne
