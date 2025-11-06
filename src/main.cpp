#include "../include/Color.hpp"
#include "../include/Resolution.hpp"
#include "../include/ImageWriter.hpp"
#include "../include/formats/STBImageWriter.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace ColorGenerator;

/**
 * @brief Print usage information
 */
void printUsage(const char* programName) {
    std::cout << "Solid Color Image Generator\n\n";
    std::cout << "Usage: " << programName << " [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -c, --color <color>      Color in hex format:\n";
    std::cout << "                           - #RGB (e.g., #F0A)\n";
    std::cout << "                           - #RRGGBB (e.g., #FF5733)\n";
    std::cout << "                           - #RRGGBBAA (e.g., #FF573380 for 50% opacity)\n";
    std::cout << "                           Alpha: 00=transparent, FF=opaque\n";
    std::cout << "  -o, --output <file>      Output file path (extension determines format)\n";
    std::cout << "  -r, --resolution <WxH>   Resolution (e.g., 1920x1080)\n";
    std::cout << "  -a, --auto               Use screen resolution (default)\n";
    std::cout << "  -f, --format <format>    Output format (png, jpg, bmp)\n";
    std::cout << "                           Note: JPEG does not support transparency\n";
    std::cout << "  -q, --quality <0-100>    JPEG quality (default: 95)\n";
    std::cout << "  -h, --help               Show this help message\n\n";
    std::cout << "Presets:\n";
    std::cout << "  --hd                     1280x720\n";
    std::cout << "  --fullhd                 1920x1080\n";
    std::cout << "  --qhd                    2560x1440\n";
    std::cout << "  --4k                     3840x2160\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << programName << " -c #FF5733 -o output.png\n";
    std::cout << "  " << programName << " -c 3498DB --fullhd -o blue.jpg -q 90\n";
    std::cout << "  " << programName << " -c \"#00FF00\" -r 800x600 -o green.bmp\n";
    std::cout << "  " << programName << " -c \"#FF573380\" -o semi-transparent.png\n";
    std::cout << "  " << programName << " -c \"#0000FF40\" --fullhd -o blue-25-percent.png\n";
}

/**
 * @brief Parse resolution string (e.g., "1920x1080")
 */
Resolution parseResolution(const std::string& str) {
    size_t xPos = str.find('x');
    if (xPos == std::string::npos) {
        xPos = str.find('X');
    }

    if (xPos == std::string::npos) {
        throw std::invalid_argument("Invalid resolution format. Use WIDTHxHEIGHT (e.g., 1920x1080)");
    }

    uint32_t width = std::stoul(str.substr(0, xPos));
    uint32_t height = std::stoul(str.substr(xPos + 1));

    return Resolution(width, height);
}

/**
 * @brief Extract file extension from filename
 */
std::string getFileExtension(const std::string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos != std::string::npos) {
        return filename.substr(dotPos);
    }
    return "";
}

int main(int argc, char* argv[]) {
    try {
        // Default values
        std::string colorStr = "#000000";  // Black
        std::string outputFile;
        Resolution resolution;  // Auto-detected screen resolution by default
        bool useAutoResolution = true;
        std::string formatStr;
        int jpegQuality = 95;

        // Parse command line arguments
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            if (arg == "-h" || arg == "--help") {
                printUsage(argv[0]);
                return 0;
            }
            else if (arg == "-c" || arg == "--color") {
                if (i + 1 < argc) {
                    colorStr = argv[++i];
                } else {
                    throw std::invalid_argument("Missing color value");
                }
            }
            else if (arg == "-o" || arg == "--output") {
                if (i + 1 < argc) {
                    outputFile = argv[++i];
                } else {
                    throw std::invalid_argument("Missing output file");
                }
            }
            else if (arg == "-r" || arg == "--resolution") {
                if (i + 1 < argc) {
                    resolution = parseResolution(argv[++i]);
                    useAutoResolution = false;
                } else {
                    throw std::invalid_argument("Missing resolution value");
                }
            }
            else if (arg == "-a" || arg == "--auto") {
                useAutoResolution = true;
            }
            else if (arg == "-f" || arg == "--format") {
                if (i + 1 < argc) {
                    formatStr = argv[++i];
                } else {
                    throw std::invalid_argument("Missing format value");
                }
            }
            else if (arg == "-q" || arg == "--quality") {
                if (i + 1 < argc) {
                    jpegQuality = std::stoi(argv[++i]);
                } else {
                    throw std::invalid_argument("Missing quality value");
                }
            }
            else if (arg == "--hd") {
                resolution = Resolution::HD();
                useAutoResolution = false;
            }
            else if (arg == "--fullhd") {
                resolution = Resolution::FullHD();
                useAutoResolution = false;
            }
            else if (arg == "--qhd") {
                resolution = Resolution::QHD();
                useAutoResolution = false;
            }
            else if (arg == "--4k") {
                resolution = Resolution::UHD4K();
                useAutoResolution = false;
            }
            else {
                std::cerr << "Unknown option: " << arg << "\n";
                printUsage(argv[0]);
                return 1;
            }
        }

        // Validate required parameters
        if (outputFile.empty()) {
            std::cerr << "Error: Output file is required (-o or --output)\n\n";
            printUsage(argv[0]);
            return 1;
        }

        // Parse color
        Color color(colorStr);

        // Detect screen resolution if auto mode
        if (useAutoResolution) {
            try {
                resolution = Resolution::detectScreenResolution();
                std::cout << "Detected screen resolution: " << resolution.toString() << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Warning: Failed to detect screen resolution, using Full HD (1920x1080)\n";
                resolution = Resolution::FullHD();
            }
        }

        // Determine format from extension or format flag
        std::string extension;
        if (!formatStr.empty()) {
            extension = "." + formatStr;
        } else {
            extension = getFileExtension(outputFile);
            if (extension.empty()) {
                throw std::invalid_argument("Cannot determine output format. Specify format with -f or use file extension");
            }
        }

        // Create writer
        ImageFormatPtr writer = ImageWriter::createWriterFromExtension(extension);

        // Special handling for JPEG quality
        if (extension == ".jpg" || extension == ".jpeg") {
            STBImageWriter* stbWriter = dynamic_cast<STBImageWriter*>(writer.get());
            if (stbWriter) {
                stbWriter->setJPEGQuality(jpegQuality);
            }
        }

        // Generate image
        std::cout << "Generating " << resolution.toString()
                  << " " << writer->getFormatName()
                  << " image with color " << color.toHex(!color.isOpaque()) << "...\n";

        if (!color.isOpaque()) {
            std::cout << "Note: Color has transparency (alpha = "
                      << static_cast<int>(color.getAlpha()) << "/255)\n";
        }

        bool success = writer->write(outputFile, color, resolution);

        if (success) {
            std::cout << "Image successfully saved to: " << outputFile << "\n";
            return 0;
        } else {
            std::cerr << "Failed to write image\n";
            return 1;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
