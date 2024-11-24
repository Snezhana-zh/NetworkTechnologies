#include "info_places_model.h"
#include "utils.h"

InfoPlacesModel::InfoPlacesModel() {
    places = std::make_shared<std::vector<Place>>();
}

void InfoPlacesModel::find_description(const json& locationsJson) {
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
                    if (place_json.contains("kinds")) {
                        std::string kinds = properties["kinds"];
                        place.osm = kinds;
                    }
                    if (place_json.contains("rate") && place_json["rate"].is_string()) {
                        place.rating = mapRating(place_json["rate"]);
                    }

                    if (place_json.contains("image") && place_json["image"].is_string()) {
                        place.image_url = place_json["image"];
                    }
                }
            }

            if (place.name.empty()) continue;
            place.index = ++global_index;
            places->push_back(place);
        }
    }
}

void InfoPlacesModel::find_places(const json& location) {
    std::string url;

    double lat = location["point"]["lat"];
    double lng = location["point"]["lng"];

    url = "http://api.opentripmap.com/0.1/" + locale + "/places/radius?radius=1000&lon=" + std::to_string(lng) +
        "&lat=" + std::to_string(lat) +
        "&apikey=" + places_api_key + "&limit=" + std::to_string(limit);

    std::string response = performRequest(url);
    auto locationsJson = json::parse(response);

    find_description(locationsJson);
}

std::shared_ptr<std::vector<Place>> InfoPlacesModel::getInfoPlaces(const json& location) {
    find_places(location);

    return places;
}