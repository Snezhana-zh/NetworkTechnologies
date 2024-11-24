#include "view.h"
#include "utils.h"

std::string View::createResultInfoHtml(const WeatherData& weatherData, std::vector<Place> places, Place loc) {
    std::string html = R"(
        <!DOCTYPE html>
        <html>
        <head>
            <meta charset="UTF-8">
            <title>Information</title>
            <script>
                function goBack() {
                    window.history.back();
                }
            </script>
        </head>
        <body>
            <h1>)";
    html += loc.name + ", " + loc.osm + ", " + loc.country + R"(
            <br><button onclick=goBack()>go back</button>
            <h1>Weather Information</h1>
            <p>Temperature: )" + formatDouble(weatherData.temperature - ABSOLUTE_ZERO) + " C" + R"(</p>
            <p>Feels like: )" + formatDouble(weatherData.feels_like - ABSOLUTE_ZERO) + " C" + R"(</p>
            <p>Min Temperature: )" + formatDouble(weatherData.temp_min - ABSOLUTE_ZERO) + " C" + R"(</p>
            <p>Max Temperature: )" + formatDouble(weatherData.temp_max - ABSOLUTE_ZERO) + " C" + R"(</p>
            <p>Pressure: )" + formatDouble(weatherData.pressure * hPa_const) + " mmHg" + R"(</p>
            <p>Humidity: )" + formatDouble(weatherData.humidity) + "%" + R"(</p>
            <p>Wind Speed: )" + formatDouble(weatherData.wind_speed) + " m/s" + R"(</p>
            <p>Cloudiness: )" + formatDouble(weatherData.cloudiness) + "%" + R"(</p>
            <p>Visibility: )" + formatDouble(weatherData.visibility) + " meters" + R"(</p>
            
            <h1>Interesting places</h1>)";

    for (const auto& place : places) {
        html += "<h2>" + std::to_string(place.index) + ") " + place.name + ", rating: " + place.rating + R"(</h2>
        <div>)" + place.description + R"(</div>
            <img src =")" + place.image_url + R"(" alt=")" + place.name + R"(" height="400">)";
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
            <script>
                function goBack() {
                    window.history.back();
                }
            </script>
        </head>
        <body>
            <h1>Locations Found</h1>
            <ul>
    )";
    for (size_t i = 0; i < locations_list.size(); ++i) {
        const auto& loc = locations_list[i];
        html += "<li><a href=\"/info?location=" + location + "&id=" + std::to_string(i) + "\">" + 
            loc["name"].get<std::string>() + ", " + loc["osm_value"].get<std::string>() + ", " + loc["country"].get<std::string>() + "</a></li>";
    }
    html += R"(
            </ul>
        <button onclick=goBack()>go back</button>
        </body>
        </html>
    )";
    return html;
}