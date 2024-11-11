//
// Created by Amina on 07/11/2024.
//
#include "resize.hpp"
#include "imagesoa.hpp" // Asegúrate de incluir este archivo
#include <cmath> // Asegúrate de incluir esta biblioteca

// Interpolación para valores de 8 bits
uint8_t interpolate8SOA(uint8_t v00, uint8_t v01, float ttt) {
    return static_cast<uint8_t>((static_cast<float>(v00) * (1.0F - ttt)) + (static_cast<float>(v01) * ttt));
}

// Interpolación para valores de 16 bits
uint16_t interpolate16SOA(uint16_t v00, uint16_t v01, float ttt) {
    return static_cast<uint16_t>((static_cast<float>(v00) * (1.0F - ttt)) + (static_cast<float>(v01) * ttt));
}

// Interpolación de un píxel genérico
template<typename T>
T interpolatePixelSOA(T v00, T v01, float ttt) {
    return static_cast<T>((static_cast<float>(v00) * (1.0F - ttt)) + (static_cast<float>(v01) * ttt));
}

// Obtener un píxel de la imagen
template<typename T>
T getPixelSOA(const std::vector<T>& pixels, size_t width, size_t hor, size_t ver) {
    return pixels[(ver * width) + hor];
}

// Redimensionar los píxeles de la imagen
template<typename T>
std::vector<T> resizePixelsSOA(const std::vector<T>& srcPixels, const PPMMetadata& metadata, size_t newWidth, size_t newHeight) {
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

            T color1 = interpolatePixelSOA(
                getPixelSOA(srcPixels, metadata.width, xlow, ylow),
                getPixelSOA(srcPixels, metadata.width, xhigh, ylow),
                static_cast<float>(xOrig - static_cast<double>(xlow))
            );

            T color2 = interpolatePixelSOA(
                getPixelSOA(srcPixels, metadata.width, xlow, yhigh),
                getPixelSOA(srcPixels, metadata.width, xhigh, yhigh),
                static_cast<float>(xOrig - static_cast<double>(xlow))
            );

            dstPixels[(y_prime * newWidth) + x_prime] = interpolatePixelSOA(color1, color2, static_cast<float>(yOrig - static_cast<double>(ylow)));
        }
    }
    return dstPixels;
}

// Función principal para redimensionar la imagen en formato SOA
ImageSOA resizeSOA(const ImageSOA& srcImage, const PPMMetadata& metadata, const std::vector<size_t>& newSize, const std::string& outputPath) {
    ImageSOA dstImage;
    size_t const newWidth = newSize[0];
    size_t const newHeight = newSize[1];

    if (std::holds_alternative<std::vector<uint8_t>>(srcImage.redChannel)) {
        // Antes del redimensionado: Tamaños de los canales en la imagen de entrada
        const auto& redChannel = std::get<std::vector<uint8_t>>(srcImage.redChannel);
        const auto& greenChannel = std::get<std::vector<uint8_t>>(srcImage.greenChannel);
        const auto& blueChannel = std::get<std::vector<uint8_t>>(srcImage.blueChannel);

        // Redimensionar los canales
        dstImage.redChannel = resizePixelsSOA(redChannel, metadata, newWidth, newHeight);
        dstImage.greenChannel = resizePixelsSOA(greenChannel, metadata, newWidth, newHeight);
        dstImage.blueChannel = resizePixelsSOA(blueChannel, metadata, newWidth, newHeight);

    } else if (std::holds_alternative<std::vector<uint16_t>>(srcImage.redChannel)) {
        dstImage.redChannel = resizePixelsSOA(std::get<std::vector<uint16_t>>(srcImage.redChannel), metadata, newWidth, newHeight);
        dstImage.greenChannel = resizePixelsSOA(std::get<std::vector<uint16_t>>(srcImage.greenChannel), metadata, newWidth, newHeight);
        dstImage.blueChannel = resizePixelsSOA(std::get<std::vector<uint16_t>>(srcImage.blueChannel), metadata, newWidth, newHeight);
    }
    // Crear un nuevo metadata con el tamaño actualizado
    PPMMetadata newMetadata = metadata;
    newMetadata.width = newWidth;
    newMetadata.height = newHeight;

    saveSOAtoPPM(dstImage, newMetadata, newMetadata.max_value, outputPath);
    std::cout << "La imagen con el nuevo tamaño se ha guardado en " << outputPath << '\n';
    return dstImage;
}
