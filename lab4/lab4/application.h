#pragma once

#include "weather.h"
#include "place.h"
#include "config.h"

class Application {
public:
    void find_location(const std::string& location_name, std::vector<json>& locations_list);

    WeatherData find_weather(const json& location_name);

    void find_places(const json& location, std::vector<Place>& places);

    void find_description(const json& locationsJson, std::vector<Place>& places);
};