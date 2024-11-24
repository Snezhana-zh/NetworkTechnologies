#include "weather_model.h"

WeatherModel::WeatherModel() {
    weather = std::make_shared<WeatherData>();
}

void WeatherModel::find_weather(const json& location) {
    double lat = location["point"]["lat"];
    double lng = location["point"]["lng"];
    std::string weatherUrl = "https://api.openweathermap.org/data/2.5/weather?lat=" + std::to_string(lat) + "&lon=" + std::to_string(lng) + "&appid=" + weather_api_key;
    std::string weatherResponse = performRequest(weatherUrl);

    auto weatherJson = json::parse(weatherResponse);
    parseToWeatherData(weatherJson, weather);
}

std::shared_ptr<WeatherData> WeatherModel::getWeather(const json& location) {
    find_weather(location);
	return weather;
}