#pragma once
#include "weather.h"

// Функция для заполнения структуры данными о погоде
WeatherData parseWeatherData(const json& weatherJson) {
    WeatherData weatherData;

    if (weatherJson.contains("main") && weatherJson["main"].is_object()) {
        const auto& main = weatherJson["main"];
        weatherData.temperature = main["temp"];
        weatherData.feels_like = main["feels_like"];
        weatherData.temp_min = main["temp_min"];
        weatherData.temp_max = main["temp_max"];
        weatherData.pressure = main["pressure"];
        weatherData.humidity = main["humidity"];
    }

    if (weatherJson.contains("wind") && weatherJson["wind"].is_object()) {
        const auto& wind = weatherJson["wind"];
        weatherData.wind_speed = wind["speed"];
    }

    if (weatherJson.contains("clouds") && weatherJson["clouds"].is_object()) {
        const auto& clouds = weatherJson["clouds"];
        weatherData.cloudiness = clouds["all"];
    }

    if (weatherJson.contains("visibility")) {
        weatherData.visibility = weatherJson["visibility"];
    }

    return weatherData;
}

// Функция для вывода информации о погоде
void printWeatherData(const WeatherData& weatherData) {
    std::cout << "------------------weather data----------------------" << std::endl;
    std::cout << "Temperature: " << weatherData.temperature - ABSOLUTE_ZERO << " C" << std::endl;
    std::cout << "Feels like: " << weatherData.feels_like - ABSOLUTE_ZERO << " C" << std::endl;
    std::cout << "Min Temperature: " << weatherData.temp_min - ABSOLUTE_ZERO << " C" << std::endl;
    std::cout << "Max Temperature: " << weatherData.temp_max - ABSOLUTE_ZERO << " C" << std::endl;
    std::cout << "Pressure: " << weatherData.pressure << " hPa" << std::endl;
    std::cout << "Humidity: " << weatherData.humidity << "%" << std::endl;
    std::cout << "Wind Speed: " << weatherData.wind_speed << " m/s" << std::endl;
    std::cout << "Cloudiness: " << weatherData.cloudiness << "%" << std::endl;
    std::cout << "Visibility: " << weatherData.visibility << " meters" << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;
}