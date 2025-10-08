#ifndef RESOLUTION_HPP
#define RESOLUTION_HPP

#include <cstdint>
#include <stdexcept>
#include <string>

namespace ColorGenerator {

/**
 * @brief Represents image resolution with width and height
 *
 * Supports auto-detection from screen resolution or manual specification.
 * Provides validation for reasonable resolution values.
 */
class Resolution {
public:
    /**
     * @brief Default constructor - uses screen resolution
     */
    Resolution();

    /**
     * @brief Construct with specific dimensions
     * @param width Width in pixels
     * @param height Height in pixels
     * @throws std::invalid_argument if dimensions invalid
     */
    Resolution(uint32_t width, uint32_t height);

    // Getters
    uint32_t getWidth() const { return width_; }
    uint32_t getHeight() const { return height_; }

    /**
     * @brief Get total pixel count
     * @return width * height
     */
    uint64_t getPixelCount() const { return static_cast<uint64_t>(width_) * height_; }

    /**
     * @brief Get aspect ratio as string
     * @return Aspect ratio (e.g., "16:9", "4:3")
     */
    std::string getAspectRatio() const;

    /**
     * @brief Detect screen resolution
     * @return Resolution object with screen dimensions
     * @throws std::runtime_error if detection fails
     */
    static Resolution detectScreenResolution();

    /**
     * @brief Validate resolution dimensions
     * @param width Width to validate
     * @param height Height to validate
     * @return true if valid
     */
    static bool isValid(uint32_t width, uint32_t height);

    /**
     * @brief String representation
     * @return String like "1920x1080"
     */
    std::string toString() const;

    // Common presets
    static Resolution HD();      // 1280x720
    static Resolution FullHD();  // 1920x1080
    static Resolution QHD();     // 2560x1440
    static Resolution UHD4K();   // 3840x2160

private:
    uint32_t width_;
    uint32_t height_;

    static constexpr uint32_t MIN_DIMENSION = 1;
    static constexpr uint32_t MAX_DIMENSION = 65535;

    /**
     * @brief Platform-specific screen resolution detection
     * @param width Output width
     * @param height Output height
     * @return true if successful
     */
    static bool getPlatformScreenResolution(uint32_t& width, uint32_t& height);

    /**
     * @brief Calculate GCD for aspect ratio calculation
     */
    static uint32_t gcd(uint32_t a, uint32_t b);
};

} // namespace ColorGenerator

#endif // RESOLUTION_HPP
