#pragma once
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const double ABSOLUTE_ZERO = 273.15;

// Структура для хранения данных о погоде
struct WeatherData {
    double temperature;
    double feels_like;
    double temp_min;
    double temp_max;
    int pressure;
    int humidity;
    double wind_speed;
    int cloudiness;
    int visibility;
};

// Функция для заполнения структуры данными о погоде
WeatherData parseWeatherData(const json& weatherJson);
// Функция для вывода информации о погоде
void printWeatherData(const WeatherData& weatherData);
