#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../include/stb_image_write.h"
#include "../../include/formats/STBImageWriter.hpp"
#include <vector>
#include <stdexcept>

namespace ColorGenerator {

STBImageWriter::STBImageWriter(Format format, int jpegQuality)
    : format_(format), jpegQuality_(validateQuality(jpegQuality)) {}

void STBImageWriter::setJPEGQuality(int quality) {
    jpegQuality_ = validateQuality(quality);
}

int STBImageWriter::validateQuality(int quality) {
    if (quality < 0) return 0;
    if (quality > 100) return 100;
    return quality;
}

std::string STBImageWriter::getFormatName() const {
    switch (format_) {
        case Format::PNG:  return "PNG";
        case Format::JPEG: return "JPEG";
        case Format::BMP:  return "BMP";
        default:           return "Unknown";
    }
}

std::string STBImageWriter::getExtension() const {
    switch (format_) {
        case Format::PNG:  return ".png";
        case Format::JPEG: return ".jpg";
        case Format::BMP:  return ".bmp";
        default:           return "";
    }
}

bool STBImageWriter::supportsTransparency() const {
    // PNG and BMP support transparency via stb_image_write
    // JPEG does not support transparency
    return format_ == Format::PNG || format_ == Format::BMP;
}

void STBImageWriter::fillPixelBuffer(std::vector<uint8_t>& buffer,
                                     const Color& color,
                                     const Resolution& resolution,
                                     int channels) const {
    uint32_t width = resolution.getWidth();
    uint32_t height = resolution.getHeight();
    size_t pixelCount = static_cast<size_t>(width) * height;

    buffer.resize(pixelCount * channels);

    uint8_t r = color.getRed();
    uint8_t g = color.getGreen();
    uint8_t b = color.getBlue();
    uint8_t a = color.getAlpha();

    // Fill buffer with solid color
    for (size_t i = 0; i < pixelCount; ++i) {
        buffer[i * channels + 0] = r;
        buffer[i * channels + 1] = g;
        buffer[i * channels + 2] = b;
        if (channels == 4) {
            buffer[i * channels + 3] = a;
        }
    }
}

bool STBImageWriter::write(const std::string& filename,
                           const Color& color,
                           const Resolution& resolution) {
    uint32_t width = resolution.getWidth();
    uint32_t height = resolution.getHeight();

    // Determine number of channels based on format and transparency
    int channels;
    if (format_ == Format::JPEG) {
        channels = 3; // JPEG doesn't support alpha
    } else {
        channels = color.isOpaque() ? 3 : 4;
    }

    // Allocate and fill pixel buffer
    std::vector<uint8_t> pixels;
    fillPixelBuffer(pixels, color, resolution, channels);

    // Write image using stb_image_write
    int result = 0;
    switch (format_) {
        case Format::PNG:
            result = stbi_write_png(filename.c_str(), width, height,
                                   channels, pixels.data(), width * channels);
            break;

        case Format::JPEG:
            result = stbi_write_jpg(filename.c_str(), width, height,
                                   channels, pixels.data(), jpegQuality_);
            break;

        case Format::BMP:
            result = stbi_write_bmp(filename.c_str(), width, height,
                                   channels, pixels.data());
            break;

        default:
            throw std::runtime_error("Unsupported image format");
    }

    if (result == 0) {
        throw std::runtime_error("Failed to write image file: " + filename);
    }

    return true;
}

} // namespace ColorGenerator
