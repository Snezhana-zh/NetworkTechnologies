#include "mvc.h"
#include "weather.h"

Controller::Controller(Application* app) : application(app) {};

void Controller::start() {

    // while(true) ... ? или не надо, потому что будет Наблюдатель, реагирующий на события

    std::string location_name = input_name_of_location();

    std::vector<json> locations_list;
    application->find_location(location_name, locations_list);

    // Вывод списка локаций
    std::cout << "Locations found:" << std::endl;
    int index = 1;
    for (const auto& loc : locations_list) {
        std::cout << index++ << ") " << "Name: " << loc["name"] << ", " << "Country: " << loc["country"] << std::endl;
    }
    json selected_location;
    choose_location(selected_location, locations_list);

    WeatherData wd = application->find_weather(selected_location);
    printWeatherData(wd);

    application->find_places(selected_location);

    //std::vector<awaitable<json>> place_description_tasks;
    //for (const auto& place_name : places_task) {
    //    place_description_tasks.push_back(application->find_place_description(place_name));
    //}

    //// Сбор итоговых данных
    //auto weather = co_await gather_single_value(weather_task);
    //auto places = co_await gather_values_list(place_description_tasks);

    //// Вывод результатов
    //std::cout << "Weather: " << weather.dump() << std::endl;
    //std::cout << "Places:" << std::endl;
    //for (const auto& description : place_descriptions) {
    //    std::cout << description.dump() << std::endl;
    //}

    //view.show_weather(weather);
    //view.show_places(places);
}

std::string Controller::input_name_of_location() {
    std::string location_name;
    std::cout << "Enter location name: ";
    std::cin >> location_name;
    return location_name;
}

void Controller::choose_location(json& selected_location, std::vector<json>& locations_list) {
    size_t location_index;
    std::cout << "Select location index: ";
    std::cin >> location_index;

    if (location_index > locations_list.size()) {
        std::cout << "Wrong index." << std::endl;
        return;
    }

    selected_location = locations_list.at(location_index);
}

Application::Application(io_context& io_context) : io_context_(io_context) {}

void Application::find_location(const std::string& location_name, std::vector<json>& location_list) {
    std::string locationsUrl = "https://graphhopper.com/api/1/geocode?q=" + location_name + "&locale=en&key=" + location_api_key;
    std::string locationsResponse = performRequest(locationsUrl);

    auto locationsJson = json::parse(locationsResponse);
    location_list = locationsJson["hits"];
}

WeatherData Application::find_weather(const json& location) {
    double lat = location["point"]["lat"];
    double lng = location["point"]["lng"];
    std::string weatherUrl = "https://api.openweathermap.org/data/2.5/weather?lat=" + std::to_string(lat) + "&lon=" + std::to_string(lng) + "&appid=" + weather_api_key;
    std::string weatherResponse = performRequest(weatherUrl);

    auto weatherJson = json::parse(weatherResponse);
    return parseWeatherData(weatherJson);
}

void Application::find_places(const json& location) {
    std::string url;
    if (location.contains("extent") && location["extent"].is_array() && location["extent"].size() == 4) {
        double lon_min = location["extent"][0];
        double lat_min = location["extent"][1];
        double lon_max = location["extent"][2];
        double lat_max = location["extent"][3];

        url = "http://api.opentripmap.com/0.1/ru/places/bbox?lon_min=" + std::to_string(lon_min) +
            "&lat_min=" + std::to_string(lat_min) +
            "&lon_max=" + std::to_string(lon_max) +
            "&lat_max=" + std::to_string(lat_max) +
            "&kinds=churches&format=geojson&apikey=" + places_api_key;
    }
    else {
        std::cerr << "Invalid extent data in location JSON." << std::endl;
        return;
    }

    std::string response = performRequest(url);

    auto locationsJson = json::parse(response);

    // ...
}

//class View {
//public:
//    View(Application* app) : application(app) {}
//
//    void show_weather(const json& weather) {
//        std::cout << "Weather: " << weather.dump() << std::endl;
//    }
//
//    void show_places(const std::vector<json>& places) {
//        std::cout << "Places: " << std::endl;
//        for (const auto& place : places) {
//            std::cout << place.dump() << std::endl;
//        }
//    }
//private:
//    Application* application;
//};