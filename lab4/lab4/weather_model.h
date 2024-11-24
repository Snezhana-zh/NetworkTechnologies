#pragma once

#include "weather.h"
#include "place.h"
#include "config.h"
#include <memory>

class WeatherModel {
public:
    WeatherModel();

    std::shared_ptr<WeatherData> getWeather(const json& location);
private:
    void find_weather(const json& location);

    std::shared_ptr<WeatherData> weather;
};