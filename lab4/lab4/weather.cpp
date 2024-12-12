#pragma once
#include "weather.h"

void parseToWeatherData(const json& weatherJson, std::shared_ptr<WeatherData> weatherData) {
    if (weatherJson.contains("main") && weatherJson["main"].is_object()) {
        const auto& main = weatherJson["main"];
        weatherData->temperature = main["temp"];
        weatherData->feels_like = main["feels_like"];
        weatherData->temp_min = main["temp_min"];
        weatherData->temp_max = main["temp_max"];
        weatherData->pressure = main["pressure"];
        weatherData->humidity = main["humidity"];
    }

    if (weatherJson.contains("wind") && weatherJson["wind"].is_object()) {
        const auto& wind = weatherJson["wind"];
        weatherData->wind_speed = wind["speed"];
    }

    if (weatherJson.contains("clouds") && weatherJson["clouds"].is_object()) {
        const auto& clouds = weatherJson["clouds"];
        weatherData->cloudiness = clouds["all"];
    }

    if (weatherJson.contains("visibility")) {
        weatherData->visibility = weatherJson["visibility"];
    }
}