#pragma once

#define CURL_STATICLIB

#include <string>
#include <nlohmann/json.hpp>
#include "httplib.h"

#include "get_handler.h"

const std::string location_api_key = "60fe12b7-4d80-4ea0-b32a-00249e501f5f";
const std::string weather_api_key = "b95d544bbdb676ec90adf52d9c8354de";
const std::string places_api_key = "5ae2e3f221c38a28845f05b6525262d2e9501766df7dc543333148a9";

using json = nlohmann::json;

const std::string locale = "ru";

const int limit = 10;