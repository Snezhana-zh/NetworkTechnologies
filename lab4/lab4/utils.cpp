#include "utils.h"

std::string url_encode(const std::string& decoded) {
    const auto encoded_value = curl_easy_escape(nullptr, decoded.c_str(), static_cast<int>(decoded.length()));
    std::string result(encoded_value);
    curl_free(encoded_value);
    return result;
}

std::string url_decode(const std::string& encoded) {
    int output_length;
    const auto decoded_value = curl_easy_unescape(nullptr, encoded.c_str(), static_cast<int>(encoded.length()), &output_length);
    std::string result(decoded_value, output_length);
    curl_free(decoded_value);
    return result;
}

std::string formatDouble(double val) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << val;
    return oss.str();
}

std::string mapRating(const std::string& rating) {
    static std::unordered_map<std::string, std::string> ratingMap = {
        {"0", "1"},
        {"1", "2"},
        {"2", "3"},
        {"3", "4"},
        {"1h", "5"},
        {"2h", "5"},
        {"3h", "5"}
    };

    if (ratingMap.find(rating) != ratingMap.end()) {
        return ratingMap[rating];
    }
    return "unknown";  // По умолчанию возвращаем unknown, если рейтинг неизвестен
}