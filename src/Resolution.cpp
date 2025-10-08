#include "../include/Resolution.hpp"
#include <sstream>

// Platform-specific headers for screen resolution detection
#ifdef _WIN32
    #include <windows.h>
#elif __APPLE__
    #include <ApplicationServices/ApplicationServices.h>
#elif __linux__
    #include <X11/Xlib.h>
    #include <X11/extensions/Xrandr.h>
#endif

namespace ColorGenerator {

Resolution::Resolution() {
    // Use screen resolution by default
    try {
        *this = detectScreenResolution();
    } catch (...) {
        // Fallback to Full HD if detection fails
        width_ = 1920;
        height_ = 1080;
    }
}

Resolution::Resolution(uint32_t width, uint32_t height)
    : width_(width), height_(height) {
    if (!isValid(width, height)) {
        throw std::invalid_argument("Invalid resolution dimensions");
    }
}

bool Resolution::isValid(uint32_t width, uint32_t height) {
    return width >= MIN_DIMENSION && width <= MAX_DIMENSION &&
           height >= MIN_DIMENSION && height <= MAX_DIMENSION;
}

std::string Resolution::toString() const {
    std::ostringstream oss;
    oss << width_ << "x" << height_;
    return oss.str();
}

std::string Resolution::getAspectRatio() const {
    uint32_t divisor = gcd(width_, height_);
    std::ostringstream oss;
    oss << (width_ / divisor) << ":" << (height_ / divisor);
    return oss.str();
}

uint32_t Resolution::gcd(uint32_t a, uint32_t b) {
    while (b != 0) {
        uint32_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

bool Resolution::getPlatformScreenResolution(uint32_t& width, uint32_t& height) {
#ifdef _WIN32
    // Windows implementation
    HDC hdc = GetDC(NULL);
    if (hdc) {
        width = GetDeviceCaps(hdc, HORZRES);
        height = GetDeviceCaps(hdc, VERTRES);
        ReleaseDC(NULL, hdc);
        return true;
    }
    return false;

#elif __APPLE__
    // macOS implementation
    CGDirectDisplayID displayID = CGMainDisplayID();
    width = CGDisplayPixelsWide(displayID);
    height = CGDisplayPixelsHigh(displayID);
    return true;

#elif __linux__
    // Linux X11 implementation
    Display* display = XOpenDisplay(NULL);
    if (display) {
        Screen* screen = DefaultScreenOfDisplay(display);
        width = WidthOfScreen(screen);
        height = HeightOfScreen(screen);
        XCloseDisplay(display);
        return true;
    }
    return false;

#else
    // Unsupported platform
    return false;
#endif
}

Resolution Resolution::detectScreenResolution() {
    uint32_t width, height;

    if (getPlatformScreenResolution(width, height)) {
        if (isValid(width, height)) {
            return Resolution(width, height);
        }
    }

    throw std::runtime_error("Failed to detect screen resolution");
}

// Common presets
Resolution Resolution::HD() {
    return Resolution(1280, 720);
}

Resolution Resolution::FullHD() {
    return Resolution(1920, 1080);
}

Resolution Resolution::QHD() {
    return Resolution(2560, 1440);
}

Resolution Resolution::UHD4K() {
    return Resolution(3840, 2160);
}

} // namespace ColorGenerator
