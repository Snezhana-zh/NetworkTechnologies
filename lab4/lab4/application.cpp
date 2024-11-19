#include "application.h"
#include "utils.h"

// acync mode!!!
// ru language
// how html + start() works ?

void Application::find_location(const std::string& location_name_arg, std::vector<json>& location_list) {
    std::string location_name = location_name_arg;
    std::replace(location_name.begin(), location_name.end(), ' ', '+');

    std::string locationsUrl = "https://graphhopper.com/api/1/geocode?q=" + url_encode(location_name) + "&locale=" + locale + "&key=" + location_api_key;
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

void Application::find_description(const json& locationsJson, std::vector<Place>& places) {
    std::string xid;
    size_t global_index = 0;

    if (locationsJson.contains("features") && locationsJson["features"].is_array()) {
        for (const auto& feature : locationsJson["features"]) {
            Place place;
            if (feature.contains("properties") && feature["properties"].is_object()) {
                const auto& properties = feature["properties"];
                if (properties.contains("name") && properties["name"].is_string()) {
                    place.name = properties["name"];
                }
                if (properties.contains("xid")) {
                    xid = properties["xid"];
                    std::string url = "http://api.opentripmap.com/0.1/" + locale + "/places/xid/" +
                        xid + "?apikey=" + places_api_key;

                    std::string response = performRequest(url);

                    auto place_json = json::parse(response);

                    if (place_json.contains("wikipedia_extracts")) {
                        const auto& properties = place_json["wikipedia_extracts"];
                        if (properties.contains("html")) {
                            place.description = properties["html"];
                        }
                    }
                }
                else {
                    std::cerr << "Can't found xid." << std::endl;
                    return;
                }
            }

            place.index = ++global_index;
            places.push_back(place);

            if (places.size() >= 10) break;
        }
    }
}

void Application::find_places(const json& location, std::vector<Place>& places) {
    std::string url;
    if (location.contains("extent") && location["extent"].is_array() && location["extent"].size() == 4) {
        double lon_min = location["extent"][0];
        double lat_min = location["extent"][1];
        double lon_max = location["extent"][2];
        double lat_max = location["extent"][3];
        url = "http://api.opentripmap.com/0.1/" + locale +  "/places/bbox?lon_min=" + std::to_string(lon_min) +
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

    find_description(locationsJson, places);
}