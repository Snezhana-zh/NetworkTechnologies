#include "location_model.h"
#include "utils.h"

LocationsModel::LocationsModel() {
    locations_list = std::make_shared<std::vector<json>>();
}

void LocationsModel::find_locations_by_name(const std::string& location_name_arg) {
    std::string location_name = location_name_arg;
    std::replace(location_name.begin(), location_name.end(), ' ', '+');

    std::string locationsUrl = "https://graphhopper.com/api/1/geocode?q=" + url_encode(location_name) + "&locale=" + locale + "&key=" + location_api_key + "&limit=" + std::to_string(limit);
    std::string locationsResponse = performRequest(locationsUrl);

    auto locationsJson = json::parse(locationsResponse);
    locations_list->assign(locationsJson["hits"].begin(), locationsJson["hits"].end());
}

std::shared_ptr<std::vector<json>> LocationsModel::getLocations(const std::string& location_name_arg) {
    find_locations_by_name(location_name_arg);

    return locations_list;
}