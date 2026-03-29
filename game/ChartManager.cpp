#include "ChartManager.hpp"


namespace ne {

ChartManager::ChartManager(const YAML::Node& root) {
    auto hitObjects = root["HitObjects"];

    if(!hitObjects.IsDefined() || !hitObjects.IsSequence())
    {
        std::cerr << "Failed to process HitObjects catagory in YAML file\n";
        return;
    }

    m_data.reserve(hitObjects.size()); 

    for(auto hitObject : hitObjects)
    {
        auto startTime = hitObject["StartTime"].as<std::int32_t>(0);
        auto endTime = hitObject["EndTime"].as<std::int32_t>(startTime);
        auto lane = hitObject["Lane"].as<int>(0);            
        
        m_data.emplace_back(sf::milliseconds(startTime), sf::milliseconds(endTime), lane);
    }
}

ChartManager::ChartManager(const std::string& path) :
    ChartManager(YAML::LoadFile(path)) 
{}

std::span<HitObject> ChartManager::query(sf::Time startTime, sf::Time endTime) {
    if(m_data.empty()) 
    {
        return {};
    }

    auto startIt = std::find_if(m_data.begin(), m_data.end(), [=](const auto& obj) {
        return obj.endTime >= startTime;
    });

    auto endIt = std::find_if(startIt, m_data.end(), [=](const auto& obj) {
        return obj.startTime > endTime;
    });

    return {startIt, endIt};
}

std::span<const HitObject> ChartManager::query(sf::Time startTime, sf::Time endTime) const {
    if(m_data.empty()) 
    {
        return {};
    }

    auto startIt = std::find_if(m_data.begin(), m_data.end(), [=](const auto& obj) {
        return obj.endTime >= startTime;
    });

    auto endIt = std::find_if(startIt, m_data.end(), [=](const auto& obj) {
        return obj.startTime > endTime;
    });

    return {startIt, endIt};
}

} //namespace ne