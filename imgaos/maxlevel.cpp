//
// Created by asanch on 10/19/24.
//

#include "maxlevel.hpp"


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
 * @return int 0 if the image was saved successfully, 1 if there was an error opening the file or
 *             if the pixel format is not supported.
 * @throws std::runtime_error If the output file cannot be opened or if the pixel format is not supported.
 */
int maxlevelAOS(const ImageAOS& srcImage, const PPMMetadata& metadata, const int newMaxLevel, const std::string& outputPath) {
    ImageAOS max_level_AOS;
    // Cambiar el nivel máximo de los píxeles
    if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels)) {
        const auto& srcPixels = std::get<std::vector<Pixel8>>(srcImage.pixels);
        std::vector<Pixel8> out_pixels(srcPixels.size());
        for (size_t index = 0; index < srcPixels.size(); ++index) {
            const auto& [r, g, b] = srcPixels[index];
            out_pixels[index].r = static_cast<uint8_t>(r * newMaxLevel / metadata.max_value);
            out_pixels[index].g = static_cast<uint8_t>(g * newMaxLevel / metadata.max_value);
            out_pixels[index].b = static_cast<uint8_t>(b * newMaxLevel / metadata.max_value);
        }
        max_level_AOS.pixels = std::move(out_pixels);
    } else if (std::holds_alternative<std::vector<Pixel16>>(srcImage.pixels)) {
        const auto& srcPixels = std::get<std::vector<Pixel16>>(srcImage.pixels);
        std::vector<Pixel16> out_pixels(srcPixels.size());
        for (size_t index = 0; index < srcPixels.size(); ++index) {
            const auto& [r, g, b] = srcPixels[index];
            out_pixels[index].r = static_cast<uint16_t>(r * newMaxLevel / metadata.max_value);
            out_pixels[index].g = static_cast<uint16_t>(g * newMaxLevel / metadata.max_value);
            out_pixels[index].b = static_cast<uint16_t>(b * newMaxLevel / metadata.max_value);
        }
        max_level_AOS.pixels = std::move(out_pixels);
    } else {
        throw std::runtime_error("Unsupported pixel format");
    }
    // Guardar la imagen con el nuevo nivel máximo
    saveAOStoPPM(max_level_AOS, metadata, newMaxLevel, outputPath);
    return 0;
}
