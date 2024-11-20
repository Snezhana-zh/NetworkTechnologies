#include "view.h"
#include "utils.h"

std::string View::createResultInfoHtml(const WeatherData& weatherData, std::vector<Place> places) {
    std::string html = R"(
        <!DOCTYPE html>
        <html>
        <head>
            <meta charset="UTF-8">
            <title>Weather Information</title>
        </head>
        <body>
            <h1>Weather Information</h1>
            <p>Temperature: )" + std::to_string(weatherData.temperature - ABSOLUTE_ZERO) + " C" + R"(</p>
            <p>Feels like: )" + std::to_string(weatherData.feels_like - ABSOLUTE_ZERO) + " C" + R"(</p>
            <p>Min Temperature: )" + std::to_string(weatherData.temp_min - ABSOLUTE_ZERO) + " C" + R"(</p>
            <p>Max Temperature: )" + std::to_string(weatherData.temp_max - ABSOLUTE_ZERO) + " C" + R"(</p>
            <p>Pressure: )" + std::to_string(weatherData.pressure) + " hPa" + R"(</p>
            <p>Humidity: )" + std::to_string(weatherData.humidity) + "%" + R"(</p>
            <p>Wind Speed: )" + std::to_string(weatherData.wind_speed) + " m/s" + R"(</p>
            <p>Cloudiness: )" + std::to_string(weatherData.cloudiness) + "%" + R"(</p>
            <p>Visibility: )" + std::to_string(weatherData.visibility) + " meters" + R"(</p>
            
            <h1>Places Information</h1>)";

    for (const auto& place : places) {

        html += R"(<h2>)" + std::to_string(place.index) + ") " + place.name + R"(</h2>
        <div>)" + place.description + R"(</div>)";

    }

    html += R"(    
        </body>
        </html>
    )";

    return html;
}

std::string View::createIndexHtml() {
    return R"(
        <html>
        <head>
            <meta charset="UTF-8">
            <title>Locations</title>
        </head>
        <body>
            <h1>Locations Search App</h1>
            <form action="/locations" method="get">
                <input type="text" name="location" placeholder="Enter location name">
                <input type="submit" value="Find Locations">
            </form>
        </body>
        </html>
    )";
}

std::string View::createLocationsHtml(const std::string& location, const std::vector<json>& locations_list) {
    std::string html = R"(
        <html>
        <head>
            <meta charset="UTF-8">
            <title>Locations</title>
        </head>
        <body>
            <h1>Locations Found</h1>
            <ul>
    )";
    for (size_t i = 0; i < locations_list.size(); ++i) {
        const auto& loc = locations_list[i];
        html += "<li><a href=\"/info?location=" + location + "&id=" + std::to_string(i) + "\">" + loc["name"].get<std::string>() + ", " + loc["country"].get<std::string>() + "</a></li>";
    }
    html += R"(
            </ul>
        </body>
        </html>
    )";
    return html;
}