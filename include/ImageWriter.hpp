#ifndef IMAGEWRITER_HPP
#define IMAGEWRITER_HPP

#include "ImageFormat.hpp"
#include <string>
#include <memory>
#include <map>
#include <vector>

namespace ColorGenerator {

/**
 * @brief Supported image format types
 */
enum class FormatType {
    PNG,
    JPEG,
    BMP,
    // Future formats can be added here:
    // TIFF,
    // WEBP,
    // GIF
};

/**
 * @brief Factory class for creating image format writers
 *
 * Implements the Factory pattern to instantiate appropriate
 * format writers based on the requested format type.
 */
class ImageWriter {
public:
    /**
     * @brief Create an image format writer
     * @param format Format type to create
     * @return Unique pointer to format writer
     * @throws std::invalid_argument if format not supported
     */
    static ImageFormatPtr createWriter(FormatType format);

    /**
     * @brief Create writer from file extension
     * @param extension File extension (with or without dot)
     * @return Unique pointer to format writer
     * @throws std::invalid_argument if extension not recognized
     */
    static ImageFormatPtr createWriterFromExtension(const std::string& extension);

    /**
     * @brief Get format type from file extension
     * @param extension File extension
     * @return Format type
     * @throws std::invalid_argument if extension not recognized
     */
    static FormatType getFormatFromExtension(const std::string& extension);

    /**
     * @brief Get list of supported extensions
     * @return Vector of supported extensions
     */
    static std::vector<std::string> getSupportedExtensions();

    /**
     * @brief Check if format is supported
     * @param format Format type to check
     * @return true if supported
     */
    static bool isFormatSupported(FormatType format);

    /**
     * @brief Get format name as string
     * @param format Format type
     * @return Format name
     */
    static std::string getFormatName(FormatType format);

private:
    // Extension to format mapping
    static const std::map<std::string, FormatType> extensionMap_;
};

} // namespace ColorGenerator

#endif // IMAGEWRITER_HPP
