#pragma once

#include "weather.h"
#include "place.h"
#include <memory>

class View {
public:
	std::string createResultInfoHtml(const std::shared_ptr<WeatherData> weatherData, std::shared_ptr<std::vector<Place>> places, Place loc);

	std::string createIndexHtml();

	std::string createLocationsHtml(const std::string& location, const std::shared_ptr<std::vector<json>> locations_list);
};