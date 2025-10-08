#ifndef IMAGEFORMAT_HPP
#define IMAGEFORMAT_HPP

#include "Color.hpp"
#include "Resolution.hpp"
#include <string>
#include <memory>

namespace ColorGenerator {

/**
 * @brief Abstract interface for image format writers
 *
 * Implements the Strategy pattern to support multiple image formats.
 * Each format implementation handles its own encoding logic.
 */
class IImageFormat {
public:
    virtual ~IImageFormat() = default;

    /**
     * @brief Write a solid color image to file
     * @param filename Output file path
     * @param color Color to fill the image
     * @param resolution Image dimensions
     * @return true if successful
     * @throws std::runtime_error on write failure
     */
    virtual bool write(const std::string& filename,
                      const Color& color,
                      const Resolution& resolution) = 0;

    /**
     * @brief Get format name
     * @return Format name (e.g., "PNG", "JPEG", "BMP")
     */
    virtual std::string getFormatName() const = 0;

    /**
     * @brief Get file extension
     * @return Extension including dot (e.g., ".png", ".jpg")
     */
    virtual std::string getExtension() const = 0;

    /**
     * @brief Check if format supports transparency
     * @return true if alpha channel supported
     */
    virtual bool supportsTransparency() const = 0;

    /**
     * @brief Get maximum supported dimensions
     * @param maxWidth Output max width
     * @param maxHeight Output max height
     */
    virtual void getMaxDimensions(uint32_t& maxWidth, uint32_t& maxHeight) const {
        maxWidth = 65535;
        maxHeight = 65535;
    }
};

/**
 * @brief Smart pointer type for image format instances
 */
using ImageFormatPtr = std::unique_ptr<IImageFormat>;

} // namespace ColorGenerator

#endif // IMAGEFORMAT_HPP
