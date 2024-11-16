#pragma once
#define CURL_STATICLIB
#include <curl/curl.h>
#include <string>
#include <iostream>

std::string performRequest(const std::string& url);