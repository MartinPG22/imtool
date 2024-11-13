//
// Created by asanch on 10/19/24.
//

#include "maxlevel.hpp"


namespace {
    // Transformar imagen de 16-bit a 16-bit
    std::vector<Pixel16> convert16To16(const ImageAOS &srcImage,  const PPMMetadata& metadata, const int newMaxLevel) {
        const auto& srcPixels = std::get<std::vector<Pixel16>>(srcImage.pixels);
        std::vector<Pixel16> out_pixels(srcPixels.size());
        for (size_t index = 0; index < srcPixels.size(); ++index) {
            const auto& [r, g, b] = srcPixels[index];
            out_pixels[index].r = static_cast<uint16_t>(r * newMaxLevel / metadata.max_value);
            out_pixels[index].g = static_cast<uint16_t>(g * newMaxLevel / metadata.max_value);
            out_pixels[index].b = static_cast<uint16_t>(b * newMaxLevel / metadata.max_value);
        }
        return out_pixels;
    }
    // Convierte una imagen de 8-bit a 16-bit
    std::vector<Pixel8> convert8To16(const ImageAOS &srcImage,  const PPMMetadata& metadata, const int newMaxLevel) {
        const auto& srcPixels = std::get<std::vector<Pixel16>>(srcImage.pixels);
        std::vector<Pixel8> out_pixels(srcPixels.size());
        for (size_t index = 0; index < srcPixels.size(); ++index) {
            const auto& [r, g, b] = srcPixels[index];
            out_pixels[index].r = static_cast<uint8_t>(r * newMaxLevel / metadata.max_value);
            out_pixels[index].g = static_cast<uint8_t>(g * newMaxLevel / metadata.max_value);
            out_pixels[index].b = static_cast<uint8_t>(b * newMaxLevel / metadata.max_value);
        }
        return out_pixels;
    }
    // Convierte una imagen de 16-bit a 8-bit
    std::vector<Pixel16> convert16To8(const ImageAOS &srcImage,  const PPMMetadata& metadata, const int newMaxLevel) {
        const auto& srcPixels = std::get<std::vector<Pixel8>>(srcImage.pixels);
        std::vector<Pixel16> out_pixels(srcPixels.size());
        for (size_t index = 0; index < srcPixels.size(); ++index) {
            const auto& [r, g, b] = srcPixels[index];
            out_pixels[index].r = static_cast<uint16_t>(r * newMaxLevel / metadata.max_value);
            out_pixels[index].g = static_cast<uint16_t>(g * newMaxLevel / metadata.max_value);
            out_pixels[index].b = static_cast<uint16_t>(b * newMaxLevel / metadata.max_value);
        }
        return out_pixels;
    }
}

/**
 * @brief Changes the maximum pixel level of an image in AOS format and saves it in PPM.
 *
 * This function takes an image in AOS (Array of Structures) format and saves it to a file
 * in PPM (P6) format. The function can handle images with 8-bit or 16-bit pixels.
 * If a 16-bit pixel image is provided, the pixels are converted to 8 bits
 * by scaling their values according to the specified new maximum level.
 *
 * @param srcImage The image in AOS format.
 * @param metadata Metadata of the image.
 * @param newMaxLevel The new maximum level of the pixels.
 * @param outputPath The path where the image with the new maximum level will be saved.
 * @return ImageAOS The image with the new maximum level.
 * @throws std::runtime_error If the output file cannot be opened or if the pixel format is not supported.
 */
ImageAOS maxlevelAOS(const ImageAOS &srcImage, const PPMMetadata &metadata, const int newMaxLevel,
                     const std::string &outputPath) {
    if (newMaxLevel <= 0 || newMaxLevel > MAX_COLOR_VALUE_16) {
        throw std::runtime_error("Invalid max level");
    }
    ImageAOS max_level_AOS;
    // Caso 1: Píxeles de 8-bit y nuevo nivel máximo de intensidad 8-bit
    if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels) && newMaxLevel <= MAX_COLOR_VALUE) {
        const auto& srcPixels = std::get<std::vector<Pixel8>>(srcImage.pixels);
        if (metadata.width * metadata.height != srcPixels.size()) {
            throw std::runtime_error("Invalid number of pixels");
        }
        std::vector<Pixel8> out_pixels(srcPixels.size());
        for (size_t index = 0; index < srcPixels.size(); ++index) {
            const auto& [r, g, b] = srcPixels[index];
            out_pixels[index].r = static_cast<uint8_t>(r * newMaxLevel / metadata.max_value);
            out_pixels[index].g = static_cast<uint8_t>(g * newMaxLevel / metadata.max_value);
            out_pixels[index].b = static_cast<uint8_t>(b * newMaxLevel / metadata.max_value);
        }
        max_level_AOS.pixels = std::move(out_pixels);
    // Caso 2: Píxeles de 16-bit y nuevo nivel máximo de intensidad 16-bit
    } else if (std::holds_alternative<std::vector<Pixel16>>(srcImage.pixels) && newMaxLevel > MAX_COLOR_VALUE) {
        max_level_AOS.pixels = convert16To16(srcImage, metadata, newMaxLevel);
    // Caso 3: Píxeles de 8-bit y nuevo nivel máximo de intensidad 16-bit
    } else if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels) && newMaxLevel > MAX_COLOR_VALUE) {
        max_level_AOS.pixels = convert16To8(srcImage, metadata, newMaxLevel);
    // Caso 4: Píxeles de 16-bit y nuevo nivel máximo de intensidad 8-bit
    } else if (std::holds_alternative<std::vector<Pixel16>>(srcImage.pixels) && newMaxLevel <= MAX_COLOR_VALUE) {
        max_level_AOS.pixels = convert8To16(srcImage, metadata, newMaxLevel);
    } else {
        throw std::runtime_error("Unsupported pixel format");
    }
    // Guardar la imagen con el nuevo nivel máximo
    saveAOStoPPM(max_level_AOS, metadata, newMaxLevel, outputPath);
    return max_level_AOS;
}
