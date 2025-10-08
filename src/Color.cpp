#include "../include/Color.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

namespace ColorGenerator {

Color::Color() : red_(0), green_(0), blue_(0), alpha_(255) {}

Color::Color(uint8_t r, uint8_t g, uint8_t b)
    : red_(r), green_(g), blue_(b), alpha_(255) {}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : red_(r), green_(g), blue_(b), alpha_(a) {}

Color::Color(const std::string& hex) {
    parseHex(hex);
}

void Color::parseHex(const std::string& hex) {
    std::string cleaned = hex;

    // Remove '#' if present
    if (!cleaned.empty() && cleaned[0] == '#') {
        cleaned = cleaned.substr(1);
    }

    // Convert to uppercase for easier processing
    std::transform(cleaned.begin(), cleaned.end(), cleaned.begin(), ::toupper);

    // Validate hex characters
    for (char c : cleaned) {
        if (!std::isxdigit(static_cast<unsigned char>(c))) {
            throw std::invalid_argument("Invalid hex color format: contains non-hex characters");
        }
    }

    // Parse based on length
    if (cleaned.length() == 3) {
        // Short format: RGB (e.g., "F0A" -> "FF00AA")
        red_ = std::stoi(std::string(2, cleaned[0]), nullptr, 16);
        green_ = std::stoi(std::string(2, cleaned[1]), nullptr, 16);
        blue_ = std::stoi(std::string(2, cleaned[2]), nullptr, 16);
        alpha_ = 255;
    }
    else if (cleaned.length() == 6) {
        // Standard format: RRGGBB
        red_ = std::stoi(cleaned.substr(0, 2), nullptr, 16);
        green_ = std::stoi(cleaned.substr(2, 2), nullptr, 16);
        blue_ = std::stoi(cleaned.substr(4, 2), nullptr, 16);
        alpha_ = 255;
    }
    else if (cleaned.length() == 8) {
        // Extended format: RRGGBBAA
        red_ = std::stoi(cleaned.substr(0, 2), nullptr, 16);
        green_ = std::stoi(cleaned.substr(2, 2), nullptr, 16);
        blue_ = std::stoi(cleaned.substr(4, 2), nullptr, 16);
        alpha_ = std::stoi(cleaned.substr(6, 2), nullptr, 16);
    }
    else {
        throw std::invalid_argument("Invalid hex color format: must be 3, 6, or 8 characters (optionally with #)");
    }
}

std::string Color::toHex(bool includeAlpha) const {
    std::ostringstream oss;
    oss << "#"
        << std::uppercase << std::setfill('0') << std::hex
        << std::setw(2) << static_cast<int>(red_)
        << std::setw(2) << static_cast<int>(green_)
        << std::setw(2) << static_cast<int>(blue_);

    if (includeAlpha) {
        oss << std::setw(2) << static_cast<int>(alpha_);
    }

    return oss.str();
}

} // namespace ColorGenerator
