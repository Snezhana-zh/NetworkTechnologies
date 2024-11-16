#include <iostream>
#include <string>
#include <boost/asio/io_context.hpp>

#include "mvc.h"


int main() {
    try {
        io_context io_context;

        Application* application = new Application(io_context);
        // View* view = new View(application);
        Controller* controller = new Controller(application);

        controller->start();

         io_context.run();

        // delete view;
        delete application;
        delete controller;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
//}
//
//#include <iostream>
//#include <string>
//#include <vector>
//#include <curl/curl.h>
//#include <nlohmann/json.hpp>
//#include "httplib.h" // Подключаем библиотеку cpp-httplib
//
//using json = nlohmann::json;
//
//// Константа для перевода температуры из Кельвина в градусы Цельсия
//const double ABSOLUTE_ZERO = 273.15;
//
//// Структура для хранения данных о погоде
//struct WeatherData {
//    std::string description;
//    std::string main;
//    double temperature;
//    double feels_like;
//    double temp_min;
//    double temp_max;
//    int pressure;
//    int humidity;
//    double wind_speed;
//    int wind_deg;
//    int cloudiness;
//    int visibility;
//    long sunrise;
//    long sunset;
//};
//
//// Функция обратного вызова для обработки данных ответа
//size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
//    size_t newLength = size * nmemb;
//    try {
//        s->append((char*)contents, newLength);
//    }
//    catch (std::bad_alloc& e) {
//        // Обработка ошибки выделения памяти
//        return 0;
//    }
//    return newLength;
//}
//
//// Функция для выполнения HTTP-запроса
//std::string performRequest(const std::string& url) {
//    CURL* curl;
//    CURLcode res;
//    std::string readBuffer;
//
//    curl = curl_easy_init();
//    if (curl) {
//        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
//
//        res = curl_easy_perform(curl);
//
//        if (res != CURLE_OK) {
//            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
//        }
//
//        curl_easy_cleanup(curl);
//    }
//
//    return readBuffer;
//}
//
//// Функция для заполнения структуры данными о погоде
//WeatherData parseWeatherData(const json& weatherJson) {
//    WeatherData weatherData;
//
//    if (weatherJson.contains("weather") && weatherJson["weather"].is_array() && !weatherJson["weather"].empty()) {
//        const auto& weather = weatherJson["weather"][0];
//        weatherData.description = weather["description"];
//        weatherData.main = weather["main"];
//    }
//
//    if (weatherJson.contains("main") && weatherJson["main"].is_object()) {
//        const auto& main = weatherJson["main"];
//        weatherData.temperature = main["temp"] - ABSOLUTE_ZERO; // Перевод в градусы Цельсия
//        weatherData.feels_like = main["feels_like"] - ABSOLUTE_ZERO; // Перевод в градусы Цельсия
//        weatherData.temp_min = main["temp_min"] - ABSOLUTE_ZERO; // Перевод в градусы Цельсия
//        weatherData.temp_max = main["temp_max"] - ABSOLUTE_ZERO; // Перевод в градусы Цельсия
//        weatherData.pressure = main["pressure"];
//        weatherData.humidity = main["humidity"];
//    }
//
//    if (weatherJson.contains("wind") && weatherJson["wind"].is_object()) {
//        const auto& wind = weatherJson["wind"];
//        weatherData.wind_speed = wind["speed"];
//        weatherData.wind_deg = wind["deg"];
//    }
//
//    if (weatherJson.contains("clouds") && weatherJson["clouds"].is_object()) {
//        const auto& clouds = weatherJson["clouds"];
//        weatherData.cloudiness = clouds["all"];
//    }
//
//    if (weatherJson.contains("visibility")) {
//        weatherData.visibility = weatherJson["visibility"];
//    }
//
//    if (weatherJson.contains("sys") && weatherJson["sys"].is_object()) {
//        const auto& sys = weatherJson["sys"];
//        weatherData.sunrise = sys["sunrise"];
//        weatherData.sunset = sys["sunset"];
//    }
//
//    return weatherData;
//}
//
//// Функция для создания HTML-страницы с информацией о погоде
//std::string createWeatherHtml(const WeatherData& weatherData) {
//    std::string html = R"(
//        <!DOCTYPE html>
//        <html>
//        <head>
//            <title>Weather Information</title>
//        </head>
//        <body>
//            <h1>Weather Information</h1>
//            <p>Weather: )" + weatherData.description + R"(</p>
//            <p>Main: )" + weatherData.main + R"(</p>
//            <p>Temperature: )" + std::to_string(weatherData.temperature) + " °C" + R"(</p>
//            <p>Feels like: )" + std::to_string(weatherData.feels_like) + " °C" + R"(</p>
//            <p>Min Temperature: )" + std::to_string(weatherData.temp_min) + " °C" + R"(</p>
//            <p>Max Temperature: )" + std::to_string(weatherData.temp_max) + " °C" + R"(</p>
//            <p>Pressure: )" + std::to_string(weatherData.pressure) + " hPa" + R"(</p>
//            <p>Humidity: )" + std::to_string(weatherData.humidity) + "%" + R"(</p>
//            <p>Wind Speed: )" + std::to_string(weatherData.wind_speed) + " m/s" + R"(</p>
//            <p>Wind Direction: )" + std::to_string(weatherData.wind_deg) + " degrees" + R"(</p>
//            <p>Cloudiness: )" + std::to_string(weatherData.cloudiness) + "%" + R"(</p>
//            <p>Visibility: )" + std::to_string(weatherData.visibility) + " meters" + R"(</p>
//            <p>Sunrise: )" + std::to_string(weatherData.sunrise) + " UTC" + R"(</p>
//            <p>Sunset: )" + std::to_string(weatherData.sunset) + " UTC" + R"(</p>
//        </body>
//        </html>
//    )";
//
//    return html;
//}
//
//// Функция для создания HTML-страницы с формой для ввода названия места
//std::string createIndexHtml() {
//    std::string html = R"(
//        <!DOCTYPE html>
//        <html>
//        <head>
//            <title>Weather Information</title>
//        </head>
//        <body>
//            <h1>Enter a location to get weather information</h1>
//            <form action="/weather" method="get">
//                <input type="text" name="location" placeholder="Enter location">
//                <button type="submit">Get Weather</button>
//            </form>
//        </body>
//        </html>
//    )";
//
//    return html;
//}
//
//int main() {
//    httplib::Server svr;
//
//    // Обработчик для главной страницы
//    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
//        res.set_content(createIndexHtml(), "text/html");
//        });
//
//    // Обработчик для запроса погоды
//    svr.Get("/weather", [](const httplib::Request& req, httplib::Response& res) {
//        if (req.has_param("location")) {
//            std::string location = req.get_param_value("location");
//            std::string weatherUrl = "https://api.openweathermap.org/data/2.5/weather?q=" + location + "&appid=your_api_key";
//            std::string weatherResponse = performRequest(weatherUrl);
//
//            try {
//                auto weatherJson = json::parse(weatherResponse);
//                WeatherData weatherData = parseWeatherData(weatherJson);
//                std::string weatherHtml = createWeatherHtml(weatherData);
//                res.set_content(weatherHtml, "text/html");
//            }
//            catch (json::parse_error& e) {
//                std::cerr << "JSON parsing error: " << e.what() << std::endl;
//                res.set_content("Error parsing weather data", "text/plain");
//            }
//        }
//        else {
//            res.set_content("Location parameter is missing", "text/plain");
//        }
//        });
//
//    std::cout << "Server started at http://localhost:8080" << std::endl;
//    svr.listen("localhost", 8080);
//
//    return 0;
//}