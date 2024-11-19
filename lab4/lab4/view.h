#pragma once

#include "weather.h"
#include "place.h"

class View {
public:
	std::string createResultInfoHtml(const WeatherData& weatherData, std::vector<Place> places);

	std::string createIndexHtml();

	std::string createLocationsHtml(const std::string& location, const std::vector<json>& locations_list);
};