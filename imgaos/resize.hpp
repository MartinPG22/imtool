//
// Created by mapor on 21/10/2024.
//

#ifndef RESIZE_HPP
#define RESIZE_HPP

#include <iostream>  // Para std::cout y std::cerr
#include <vector>
#include <cmath>
#include "imageaos.hpp" // Asegúrate de que este archivo existe y es accesible
#include "./common/binaryio.hpp" // Asegúrate de que este archivo existe y es accesible



constexpr uint8_t MIN_PIXEL_VALUE = 8;

uint8_t interpolate8(uint8_t v00, uint8_t v01, float ttt);
uint16_t interpolate16(uint16_t v00, uint16_t v01, float ttt);
Pixel16 interpolatePixel(const Pixel16& p00, const Pixel16& p01, float ttt);
Pixel8 interpolatePixel(const Pixel8& p00, const Pixel8& p01, float ttt);

template<typename T>
T getPixel(const std::vector<T>& pixels, size_t width, size_t hor, size_t ver) {
    return pixels[(ver * width) + hor];
}
template<typename T>
std::vector<T> resizePixels(const std::vector<T>& srcPixels, const PPMMetadata& metadata, size_t newWidth, size_t newHeight) {
    std::vector<T> dstPixels(newWidth * newHeight);
    for (size_t y_prime = 0; y_prime < newHeight; ++y_prime) {
        for (size_t x_prime = 0; x_prime < newWidth; ++x_prime) {
            double const xOrig = static_cast<double>(x_prime) * static_cast<double>(metadata.width) / static_cast<double>(newWidth);
            double const yOrig = static_cast<double>(y_prime) * static_cast<double>(metadata.height) / static_cast<double>(newHeight);

            auto xlow = static_cast<size_t>(std::floor(xOrig));
            auto xhigh = static_cast<size_t>(std::ceil(xOrig));
            auto ylow = static_cast<size_t>(std::floor(yOrig));
            auto yhigh = static_cast<size_t>(std::ceil(yOrig));

            xlow = std::min(xlow, metadata.width - 1);
            xhigh = std::min(xhigh, metadata.width - 1);
            ylow = std::min(ylow, metadata.height - 1);
            yhigh = std::min(yhigh, metadata.height - 1);

            // Cambio recomendado en la función resizePixels
            T color1 = interpolatePixel(
            getPixel(srcPixels, metadata.width, xlow, ylow),
            getPixel(srcPixels, metadata.width, xhigh, ylow),
            static_cast<float>(xOrig - static_cast<double>(xlow))  // Convierte xl a double antes de la resta
            );

            T color2 = interpolatePixel(
            getPixel(srcPixels, metadata.width, xlow, yhigh),
            getPixel(srcPixels, metadata.width, xhigh, yhigh),
            static_cast<float>(xOrig - static_cast<double>(xlow))  // Convierte xl a double antes de la resta
            );

            dstPixels[(y_prime * newWidth) + x_prime] = interpolatePixel(color1, color2, static_cast<float>(yOrig - static_cast<double>(ylow)));  // Convierte yl a double
        }
    }
    return dstPixels;
}

// Declaraciones de las nuevas funciones
void savePixelsToPPM8(const std::string& outputPath, const std::vector<Pixel8>& pixels, const std::vector<size_t>& newSize, size_t intensidad);
void savePixelsToPPM16(const std::string& outputPath, const std::vector<Pixel16>& pixels, const std::vector<size_t>& newSize, size_t intensidad);

// Declaración de la función
ImageAOS resize(const ImageAOS& srcImage, const PPMMetadata& metadata, const std::vector<size_t>& newSize, const std::string& OutputPath);


#endif //RESIZE_HPP
