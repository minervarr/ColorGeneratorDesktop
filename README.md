# ColorGeneratorDesktop

A cross-platform command-line tool for generating solid-color images with support for transparency (alpha channel). Perfect for creating wallpapers, backgrounds, and test images.

## Features

- **Full Alpha Channel Support**: Create transparent or semi-transparent images using the `#RRGGBBAA` hex format
- **Multiple Color Formats**: Support for 3, 6, and 8-digit hex color codes
- **Multiple Output Formats**: PNG, JPEG, and BMP
- **Flexible Resolution Options**: Auto-detect screen resolution or use custom/preset sizes
- **Quality Control**: Adjustable JPEG quality settings
- **Cross-Platform**: Works on Windows, macOS, and Linux

## Installation

### Prerequisites

- CMake 3.15 or higher
- C++17 compatible compiler (GCC, Clang, or MSVC)

### Build Instructions

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

The executable will be created in the `build` directory.

## Usage

### Basic Syntax

```bash
./ColorImageGenerator -c <color> -o <output_file> [options]
```

### Color Format

The tool supports three hex color formats:

| Format | Example | Description |
|--------|---------|-------------|
| `#RGB` | `#F0A` | Short format (expanded to #FF00AA) |
| `#RRGGBB` | `#FF5733` | Standard 6-digit hex (fully opaque) |
| `#RRGGBBAA` | `#FF573380` | 8-digit hex with alpha channel |

#### Alpha Channel Values

The alpha channel (AA) controls transparency:

- `FF` (255) = Fully opaque (no transparency)
- `80` (128) = 50% transparent
- `40` (64) = 25% opaque / 75% transparent
- `00` (0) = Fully transparent

**Formula**: Opacity percentage = (alpha_value / 255) × 100

### Command-Line Options

| Option | Description |
|--------|-------------|
| `-c, --color <color>` | Color in hex format (required with -o) |
| `-o, --output <file>` | Output file path (required) |
| `-r, --resolution <WxH>` | Custom resolution (e.g., 1920x1080) |
| `-a, --auto` | Auto-detect screen resolution (default) |
| `-f, --format <format>` | Output format: png, jpg, or bmp |
| `-q, --quality <0-100>` | JPEG quality (default: 95) |
| `-h, --help` | Show help message |

### Resolution Presets

| Preset | Resolution | Command |
|--------|------------|---------|
| HD | 1280x720 | `--hd` |
| Full HD | 1920x1080 | `--fullhd` |
| QHD | 2560x1440 | `--qhd` |
| 4K UHD | 3840x2160 | `--4k` |

## Examples

### Basic Usage

```bash
# Simple red image (PNG, auto resolution)
./ColorImageGenerator -c "#FF0000" -o red.png

# Blue wallpaper with Full HD resolution
./ColorImageGenerator -c "#3498DB" --fullhd -o blue.jpg -q 90

# Green image with custom resolution
./ColorImageGenerator -c "#00FF00" -r 800x600 -o green.bmp
```

### Using Alpha Transparency

```bash
# Semi-transparent red (50% opacity) - PNG format
./ColorImageGenerator -c "#FF000080" -o semi-red.png

# Light blue with 25% opacity - Full HD
./ColorImageGenerator -c "#0000FF40" --fullhd -o blue-transparent.png

# Orange with 75% opacity (BF = 191)
./ColorImageGenerator -c "#FFA500BF" --4k -o orange-75.png

# Fully transparent black (useful for overlays)
./ColorImageGenerator -c "#00000000" -o transparent.png
```

### Format-Specific Examples

```bash
# PNG - Best for transparency (supports alpha channel)
./ColorImageGenerator -c "#FF573380" -o transparent.png

# JPEG - No transparency support (alpha ignored)
./ColorImageGenerator -c "#FF5733" -o opaque.jpg -q 100

# BMP - Supports transparency
./ColorImageGenerator -c "#00FF0080" -o green-transparent.bmp
```

## Alpha Channel Reference

Common alpha values and their opacity percentages:

| Hex | Decimal | Opacity | Use Case |
|-----|---------|---------|----------|
| `FF` | 255 | 100% | Fully opaque (default) |
| `E6` | 230 | 90% | Nearly opaque |
| `CC` | 204 | 80% | Slightly transparent |
| `BF` | 191 | 75% | Quarter transparent |
| `99` | 153 | 60% | Moderately transparent |
| `80` | 128 | 50% | Half transparent |
| `66` | 102 | 40% | Very transparent |
| `40` | 64 | 25% | Mostly transparent |
| `33` | 51 | 20% | Nearly transparent |
| `1A` | 26 | 10% | Very faint |
| `00` | 0 | 0% | Fully transparent |

## Format Support Comparison

| Format | Transparency | Compression | Best For |
|--------|--------------|-------------|----------|
| **PNG** | ✅ Yes (RGBA) | Lossless | Images with transparency, wallpapers |
| **JPEG** | ❌ No (RGB only) | Lossy | Photographs, opaque backgrounds |
| **BMP** | ✅ Yes (RGBA) | None | Uncompressed images, compatibility |

**Note**: When using JPEG format with an alpha channel color, the alpha value is ignored and the image will be fully opaque.

## Technical Details

### Color Class

The `Color` class (`include/Color.hpp`) provides:

- RGBA color representation (8-bit per channel)
- Hex string parsing with validation
- Hex string generation with optional alpha
- Opacity checking methods (`isOpaque()`, `isTransparent()`)

### Image Writing

The `STBImageWriter` class uses the [stb_image_write](https://github.com/nothings/stb) library to encode images:

- **PNG/BMP**: Automatically uses 4 channels (RGBA) when alpha < 255, otherwise 3 channels (RGB)
- **JPEG**: Always uses 3 channels (RGB), alpha is ignored

## Troubleshooting

### Common Issues

**Issue**: Transparency not visible
- **Solution**: Make sure you're using PNG or BMP format (JPEG doesn't support transparency)
- **Solution**: Verify the alpha value is less than FF (255)

**Issue**: Invalid hex color error
- **Solution**: Ensure hex string uses valid characters (0-9, A-F)
- **Solution**: Check format is 3, 6, or 8 characters (optionally with #)

**Issue**: Screen resolution not detected
- **Solution**: Use a preset (`--fullhd`) or specify resolution manually (`-r 1920x1080`)

## Contributing

Contributions are welcome! Please ensure your code:

- Follows the existing code style
- Includes appropriate error handling
- Works across all supported platforms

## License

This project uses the stb_image_write library, which is public domain.

## References

- [MDN Web Docs - Hex Color Notation](https://developer.mozilla.org/en-US/docs/Web/CSS/hex-color)
- [stb_image_write Library](https://github.com/nothings/stb/blob/master/stb_image_write.h)
- [Alpha Compositing (Wikipedia)](https://en.wikipedia.org/wiki/Alpha_compositing)
