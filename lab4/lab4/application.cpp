#include "application.h"

Application::Application() {
	weather = new WeatherModel();
	info = new InfoPlacesModel();
	locations = new LocationsModel();
}

Application::~Application() {
	delete weather;
	delete info;
	delete locations;
}

WeatherModel* Application::getWeatherModel() {
	return weather;
}

InfoPlacesModel* Application::getInfoModel() {
	return info;
}

LocationsModel* Application::getLocationsModel() {
	return locations;
}