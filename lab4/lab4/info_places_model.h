#pragma once

#include "weather.h"
#include "place.h"
#include "config.h"
#include <memory>

class InfoPlacesModel {
public:
    InfoPlacesModel();

    std::shared_ptr<std::vector<Place>> getInfoPlaces(const json& location);
private:
    void find_places(const json& location);

    void find_description(const json& locationsJson);

    std::shared_ptr<std::vector<Place>> places;
};