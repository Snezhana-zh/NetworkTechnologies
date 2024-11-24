#pragma once

#include "weather.h"
#include "place.h"
#include "config.h"
#include <memory>

class LocationsModel {
public:
    LocationsModel();

    std::shared_ptr<std::vector<json>> getLocations(const std::string& location_name);
private:
    void find_locations_by_name(const std::string& location_name);

    std::shared_ptr<std::vector<json>> locations_list;
};