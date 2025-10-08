#ifndef STBIMAGEWRITER_HPP
#define STBIMAGEWRITER_HPP

#include "../ImageFormat.hpp"
#include <vector>

namespace ColorGenerator {

/**
 * @brief Unified image writer using stb_image_write library
 *
 * Uses the public domain stb_image_write.h single-header library
 * for writing PNG, JPEG, and BMP formats without external dependencies.
 */
class STBImageWriter : public IImageFormat {
public:
    enum class Format {
        PNG,
        JPEG,
        BMP
    };

    /**
     * @brief Construct writer for specific format
     * @param format Image format to write
     * @param jpegQuality JPEG quality (0-100), ignored for other formats
     */
    explicit STBImageWriter(Format format, int jpegQuality = 95);
    ~STBImageWriter() override = default;

    bool write(const std::string& filename,
              const Color& color,
              const Resolution& resolution) override;

    std::string getFormatName() const override;
    std::string getExtension() const override;
    bool supportsTransparency() const override;

    void getMaxDimensions(uint32_t& maxWidth, uint32_t& maxHeight) const override {
        maxWidth = 65535;
        maxHeight = 65535;
    }

    /**
     * @brief Set JPEG quality (only affects JPEG format)
     * @param quality Quality value (0-100)
     */
    void setJPEGQuality(int quality);

    /**
     * @brief Get current JPEG quality setting
     */
    int getJPEGQuality() const { return jpegQuality_; }

private:
    Format format_;
    int jpegQuality_;

    /**
     * @brief Validate and clamp JPEG quality
     */
    static int validateQuality(int quality);

    /**
     * @brief Allocate and fill pixel buffer with solid color
     */
    void fillPixelBuffer(std::vector<uint8_t>& buffer,
                        const Color& color,
                        const Resolution& resolution,
                        int channels) const;
};

} // namespace ColorGenerator

#endif // STBIMAGEWRITER_HPP
