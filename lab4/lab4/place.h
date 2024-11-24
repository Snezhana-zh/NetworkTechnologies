#pragma once

#include <string>

struct Place {
    std::string name;
    std::string osm;
    std::string country;
    std::string description;
    std::string rating;
    std::string image_url;
    size_t index;
};