#include "controller.h"
#include "utils.h"

Controller::Controller(Application* app, View* v) : application(app), view(v) {};

void Controller::start() {

    httplib::Server svr;

    svr.Get("/", [this](const httplib::Request&, httplib::Response& res) {
        res.set_content(view->createIndexHtml(), "text/html");
        });

    svr.Get("/locations", [this](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("location")) {
            std::string location = req.get_param_value("location");
            std::vector<json> locations_list;
            application->find_location(location, locations_list);

            std::string locationsHtml = view->createLocationsHtml(url_encode(location), locations_list);
            res.set_content(locationsHtml, "text/html");
        }
        else {
            res.set_content("Location parameter is missing", "text/plain");
        }
        });

    svr.Get("/info", [this](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("location") && req.has_param("id")) {
            std::string location = req.get_param_value("location");
            std::vector<json> locations_list;
            application->find_location(location, locations_list);

            int id = std::stoi(req.get_param_value("id"));
            if (id >= 0 && id < locations_list.size()) {
                std::vector<Place> places;

                auto loc_arg = locations_list[id];
                Place loc;
                if (loc_arg.contains("name")) {
                    loc.name = loc_arg["name"];
                }
                if (loc_arg.contains("osm_value")) {
                    loc.osm = loc_arg["osm_value"];
                }
                if (loc_arg.contains("country")) {
                    loc.country = loc_arg["country"];
                }

                auto weather_future = std::async(std::launch::async, [this, loc_arg]() { return application->find_weather(loc_arg); });
                auto places_future = std::async(std::launch::async, [this, loc_arg, &places]() { return application->find_places(loc_arg, places); });
                
                places_future.wait();
                WeatherData weatherData = weather_future.get();

                try {
                    std::string infoHtml = view->createResultInfoHtml(weatherData, places, loc);
                    res.set_content(infoHtml, "text/html");
                }
                catch (json::parse_error& e) {
                    std::cerr << "JSON parsing error: " << e.what() << std::endl;
                    res.set_content("Error parsing weather data", "text/plain");
                }
            }
            else {
                res.set_content("Invalid location ID", "text/plain");
            }
        }
        else {
            res.set_content("Location or ID parameter is missing", "text/plain");
        }
        });

    std::cout << "Server started at http://localhost:8080" << std::endl;
    svr.listen("localhost", 8080);
}