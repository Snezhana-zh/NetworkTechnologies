#pragma once

#define CURL_STATICLIB
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <boost/asio/io_context.hpp>
#include <vector>

#include "get_handler.h"
#include "weather.h"

const std::string location_api_key = "60fe12b7-4d80-4ea0-b32a-00249e501f5f";
const std::string weather_api_key = "b95d544bbdb676ec90adf52d9c8354de";
const std::string places_api_key = "your_places_api_key";

using json = nlohmann::json;
using namespace boost::asio;

class Application {
public:
    Application(io_context& io_context);
    
    void find_location(const std::string& location_name, std::vector<json>& locations_list);

    WeatherData find_weather(const json& location_name);

    void find_places(const json& location);

    //awaitable<json> find_weather(const json& location) {
    //    // Implement API call to find weather
    //    co_return json::object();
    //}

    //awaitable<std::vector<std::string>> find_places(const json& location) {
    //    // Implement API call to find places
    //    co_return std::vector<std::string>();
    //}

    //awaitable<json> find_place_description(const std::string& xid) {
    //    // Implement API call to find place description
    //    co_return json::object();
    //}

private:
    io_context& io_context_;
};

class Controller {
public:
    Controller(Application* app);

    void start();

    std::string input_name_of_location();

    void choose_location(json& selected_location, std::vector<json>& locations_list);
private:
    Application* application;

    //int input_index_of_location(const std::vector<json>& locations) {
    //    int location_index;
    //    std::cout << "Enter location index: ";
    //    std::cin >> location_index;
    //    return location_index;
    //}

    //int input_number_of_places() {
    //    int places_number;
    //    std::cout << "Enter number of places: ";
    //    std::cin >> places_number;
    //    return places_number;
    //}
};

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