#include "../include/ImageWriter.hpp"
#include "../include/formats/STBImageWriter.hpp"
#include <algorithm>

namespace ColorGenerator {

// Initialize extension mapping
const std::map<std::string, FormatType> ImageWriter::extensionMap_ = {
    {"png", FormatType::PNG},
    {".png", FormatType::PNG},
    {"jpg", FormatType::JPEG},
    {".jpg", FormatType::JPEG},
    {"jpeg", FormatType::JPEG},
    {".jpeg", FormatType::JPEG},
    {"bmp", FormatType::BMP},
    {".bmp", FormatType::BMP}
};

ImageFormatPtr ImageWriter::createWriter(FormatType format) {
    switch (format) {
        case FormatType::PNG:
            return std::make_unique<STBImageWriter>(STBImageWriter::Format::PNG);
        case FormatType::JPEG:
            return std::make_unique<STBImageWriter>(STBImageWriter::Format::JPEG);
        case FormatType::BMP:
            return std::make_unique<STBImageWriter>(STBImageWriter::Format::BMP);
        default:
            throw std::invalid_argument("Unsupported format type");
    }
}

ImageFormatPtr ImageWriter::createWriterFromExtension(const std::string& extension) {
    FormatType format = getFormatFromExtension(extension);
    return createWriter(format);
}

FormatType ImageWriter::getFormatFromExtension(const std::string& extension) {
    std::string lowerExt = extension;
    std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(), ::tolower);

    auto it = extensionMap_.find(lowerExt);
    if (it != extensionMap_.end()) {
        return it->second;
    }

    throw std::invalid_argument("Unrecognized file extension: " + extension);
}

std::vector<std::string> ImageWriter::getSupportedExtensions() {
    return {".png", ".jpg", ".jpeg", ".bmp"};
}

bool ImageWriter::isFormatSupported(FormatType format) {
    switch (format) {
        case FormatType::PNG:
        case FormatType::JPEG:
        case FormatType::BMP:
            return true;
        default:
            return false;
    }
}

std::string ImageWriter::getFormatName(FormatType format) {
    switch (format) {
        case FormatType::PNG:
            return "PNG";
        case FormatType::JPEG:
            return "JPEG";
        case FormatType::BMP:
            return "BMP";
        default:
            return "Unknown";
    }
}

} // namespace ColorGenerator
