#pragma once

#include "weather.h"
#include "place.h"
#include "config.h"

#include "info_places_model.h"
#include "location_model.h"
#include "weather_model.h"

class Application {
public:
	Application();

	WeatherModel* getWeatherModel();

	InfoPlacesModel* getInfoModel();

	LocationsModel* getLocationsModel();

	~Application();
private:
	WeatherModel* weather;
	InfoPlacesModel* info;
	LocationsModel* locations;
};