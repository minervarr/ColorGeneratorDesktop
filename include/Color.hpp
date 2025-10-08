#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>
#include <string>
#include <stdexcept>

namespace ColorGenerator {

/**
 * @brief Represents an RGBA color with 8-bit channels
 *
 * Supports RGB and RGBA color representation with validation.
 * Can be constructed from individual components or hex strings.
 */
class Color {
public:
    /**
     * @brief Default constructor - creates black color
     */
    Color();

    /**
     * @brief Construct from RGB values (alpha = 255)
     * @param r Red component (0-255)
     * @param g Green component (0-255)
     * @param b Blue component (0-255)
     * @throws std::invalid_argument if values out of range
     */
    Color(uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief Construct from RGBA values
     * @param r Red component (0-255)
     * @param g Green component (0-255)
     * @param b Blue component (0-255)
     * @param a Alpha component (0-255)
     * @throws std::invalid_argument if values out of range
     */
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    /**
     * @brief Construct from hex string
     * @param hex Hex color string (formats: "#RGB", "#RRGGBB", "#RRGGBBAA")
     * @throws std::invalid_argument if invalid format
     */
    explicit Color(const std::string& hex);

    // Getters
    uint8_t getRed() const { return red_; }
    uint8_t getGreen() const { return green_; }
    uint8_t getBlue() const { return blue_; }
    uint8_t getAlpha() const { return alpha_; }

    // Setters with validation
    void setRed(uint8_t r) { red_ = r; }
    void setGreen(uint8_t g) { green_ = g; }
    void setBlue(uint8_t b) { blue_ = b; }
    void setAlpha(uint8_t a) { alpha_ = a; }

    /**
     * @brief Convert color to hex string
     * @param includeAlpha Include alpha channel in output
     * @return Hex string representation (e.g., "#FF5733" or "#FF5733FF")
     */
    std::string toHex(bool includeAlpha = false) const;

    /**
     * @brief Check if color is fully opaque
     * @return true if alpha == 255
     */
    bool isOpaque() const { return alpha_ == 255; }

    /**
     * @brief Check if color is fully transparent
     * @return true if alpha == 0
     */
    bool isTransparent() const { return alpha_ == 0; }

private:
    uint8_t red_;
    uint8_t green_;
    uint8_t blue_;
    uint8_t alpha_;

    /**
     * @brief Parse hex string to RGBA components
     * @param hex Hex string to parse
     * @throws std::invalid_argument if invalid format
     */
    void parseHex(const std::string& hex);
};

} // namespace ColorGenerator

#endif // COLOR_HPP
