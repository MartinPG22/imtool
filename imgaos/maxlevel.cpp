//
// Created by asanch on 10/19/24.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#include "imageaos.hpp"
#include "maxlevel.hpp"


void printInitialPixels(const ImageAOS& srcImage, const PPMMetadata& metadata) {
    const auto& pixels8 = std::get<std::vector<Pixel8>>(srcImage.pixels);
    for (size_t y_cord = 0; y_cord < metadata.height; ++y_cord) {
        for (size_t x_cord = 0; x_cord < metadata.width; ++x_cord) {
            size_t const index = (y_cord * metadata.width) + x_cord;
            if (index >= pixels8.size()) { break; } // Evitar overflow
            const auto& [r, g, b] = pixels8[index];
            std::cout << "(" << static_cast<int>(r) << ", "
                      << static_cast<int>(g) << ", "
                      << static_cast<int>(b) << ") ";
        }
        std::cout << '\n';
    }
}

std::vector<Pixel8> convertPixels(const ImageAOS& srcImage, int newMaxLevel, const PPMMetadata& metadata) {
    const auto& pixels8 = std::get<std::vector<Pixel8>>(srcImage.pixels);
    std::vector<Pixel8> out_pixels8(pixels8.size());

    for (size_t index = 0; index < pixels8.size(); ++index) {
        const auto& [r, g, b] = pixels8[index];
        out_pixels8[index].r = static_cast<uint8_t>(r * newMaxLevel / metadata.max_value);
        out_pixels8[index].g = static_cast<uint8_t>(g * newMaxLevel / metadata.max_value);
        out_pixels8[index].b = static_cast<uint8_t>(b * newMaxLevel / metadata.max_value);
    }

    return out_pixels8;
}

int saveImage(const std::vector<Pixel8>& pixels, const PPMMetadata& metadata, int newMaxLevel, const std::string& outputPath) {
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo de salida" << '\n';
        return 1;
    }

    // Escribir el encabezado
    outFile << "P6\n" << metadata.width << " " << metadata.height << "\n" << newMaxLevel << "\n";

    // Escribir los valores de los píxeles en el formato binario
    for (const auto& [r, g, b] : pixels) {
        outFile.put(static_cast<char>(r));
        outFile.put(static_cast<char>(g));
        outFile.put(static_cast<char>(b));
    }

    outFile.close();
    std::cout << "La imagen con el nuevo nivel máximo de intensidad se ha guardado en " << outputPath << '\n';
    return 0;
}

// Función principal refactorizada
int maxlevel(const ImageAOS& srcImage, const PPMMetadata& metadata, int newMaxLevel, const std::string& outputPath) {
    printInitialPixels(srcImage, metadata);

    const auto out_pixels8 = convertPixels(srcImage, newMaxLevel, metadata);

    return saveImage(out_pixels8, metadata, newMaxLevel, outputPath);
}
