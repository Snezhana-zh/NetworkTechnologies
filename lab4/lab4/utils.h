#pragma once

#include "config.h"

std::string url_encode(const std::string& decoded);

std::string url_decode(const std::string& encoded);

std::string formatDouble(double val);

std::string mapRating(const std::string& rating);

std::string getImage(const std::string& url);