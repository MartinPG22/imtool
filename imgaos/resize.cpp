#include <iostream>  // Agregado para std::cout y std::cerr
#include <variant>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <fstream>   // Agregado para std::ofstream
#include "resize.hpp"
#include "imageaos.hpp"
#include "./common/binaryio.hpp"

// Función de interpolación lineal entre dos valores
template<typename T>
T interpolate(T v0, T v1, float t) {
    return static_cast<T>(v0 * (1 - t) + v1 * t);
}

// Interpolación de un píxel de 8 bits
Pixel8 interpolatePixel(const Pixel8& p0, const Pixel8& p1, float t) {
    Pixel8 result;
    result.r = interpolate(p0.r, p1.r, t);
    result.g = interpolate(p0.g, p1.g, t);
    result.b = interpolate(p0.b, p1.b, t);
    return result;
}

// Interpolación de un píxel de 16 bits
Pixel16 interpolatePixel(const Pixel16& p0, const Pixel16& p1, float t) {
    Pixel16 result;
    result.r = interpolate(p0.r, p1.r, t);
    result.g = interpolate(p0.g, p1.g, t);
    result.b = interpolate(p0.b, p1.b, t);
    return result;
}

// Función para obtener un píxel a partir de las coordenadas
template<typename T>
T getPixel(const std::vector<T>& pixels, size_t width, size_t x, size_t y) {
    return pixels[y * width + x];
}

// Función para redimensionar píxeles
template<typename T>
std::vector<T> resizePixels(const std::vector<T>& srcPixels, const PPMMetadata& metadata, size_t newWidth, size_t newHeight) {
    std::vector<T> dstPixels(newWidth * newHeight);

    for (size_t y_prime = 0; y_prime < newHeight; ++y_prime) {
        for (size_t x_prime = 0; x_prime < newWidth; ++x_prime) {
            double x = static_cast<double>(x_prime) * static_cast<double>(metadata.width) / static_cast<double>(newWidth);
            double y = static_cast<double>(y_prime) * static_cast<double>(metadata.height) / static_cast<double>(newHeight);

            size_t xl = static_cast<size_t>(std::floor(x));
            size_t xh = static_cast<size_t>(std::ceil(x));
            size_t yl = static_cast<size_t>(std::floor(y));
            size_t yh = static_cast<size_t>(std::ceil(y));

            xl = std::min(xl, metadata.width - 1);
            xh = std::min(xh, metadata.width - 1);
            yl = std::min(yl, metadata.height - 1);
            yh = std::min(yh, metadata.height - 1);

            // Cambio recomendado en la función resizePixels
            T c1 = interpolatePixel(
                getPixel(srcPixels, metadata.width, xl, yl),
                getPixel(srcPixels, metadata.width, xh, yl),
                static_cast<float>(x - static_cast<double>(xl))  // Convierte xl a double antes de la resta
            );

            T c2 = interpolatePixel(
                getPixel(srcPixels, metadata.width, xl, yh),
                getPixel(srcPixels, metadata.width, xh, yh),
                static_cast<float>(x - static_cast<double>(xl))  // Convierte xl a double antes de la resta
            );

            dstPixels[y_prime * newWidth + x_prime] = interpolatePixel(c1, c2, static_cast<float>(y - static_cast<double>(yl)));  // Convierte yl a double
        }
    }
    return dstPixels;
}
// Función para imprimir los píxeles
void imprimirPixeles(const ImageAOS& image) {
    // Verifica el tipo de los píxeles
    if (std::holds_alternative<std::vector<Pixel8>>(image.pixels)) {
        const auto& pixelVector = std::get<std::vector<Pixel8>>(image.pixels);
        std::cout << "Píxeles (Pixel8):" << std::endl;

        // Imprimir solo los primeros 10 píxeles
        size_t limit = std::min(static_cast<size_t>(10), pixelVector.size());
        for (size_t i = 0; i < limit; ++i) {
            std::cout << "Píxel " << i + 1 << ": R: " << static_cast<int>(pixelVector[i].r)
                      << ", G: " << static_cast<int>(pixelVector[i].g)
                      << ", B: " << static_cast<int>(pixelVector[i].b) << std::endl;
        }
    } else if (std::holds_alternative<std::vector<Pixel16>>(image.pixels)) {
        const auto& pixelVector = std::get<std::vector<Pixel16>>(image.pixels);
        std::cout << "Píxeles (Pixel16):" << std::endl;

        // Imprimir solo los primeros 10 píxeles
        size_t limit = std::min(static_cast<size_t>(10), pixelVector.size());
        for (size_t i = 0; i < limit; ++i) {
            std::cout << "Píxel " << i + 1 << ": R: " << pixelVector[i].r
                      << ", G: " << pixelVector[i].g
                      << ", B: " << pixelVector[i].b << std::endl;
        }
    } else {
        std::cout << "No se encontró un tipo de píxel válido." << std::endl;
    }
}

// Función principal para redimensionar la imagen
ImageAOS resize(const ImageAOS& srcImage, const PPMMetadata& metadata, size_t newWidth, size_t newHeight) {
    ImageAOS dstImage;

    if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels)) {
        const auto& srcPixels = std::get<std::vector<Pixel8>>(srcImage.pixels);
        dstImage.pixels = resizePixels(srcPixels, metadata, newWidth, newHeight);
        imprimirPixeles(dstImage);
    } else if (std::holds_alternative<std::vector<Pixel16>>(srcImage.pixels)) {
        const auto& srcPixels = std::get<std::vector<Pixel16>>(srcImage.pixels);
        dstImage.pixels = resizePixels(srcPixels, metadata, newWidth, newHeight);
    }

    // Guardar el vector de píxeles en un nuevo archivo PPM
    std::ofstream outFile("imagesPPM/outputresize.ppm", std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo de salida" << std::endl;
        return dstImage; // Es bueno retornar el dstImage incluso si hubo un error
    }

    // Determina el tipo de píxel para guardar el encabezado y los datos correctamente
    if (std::holds_alternative<std::vector<Pixel8>>(dstImage.pixels)) {
        outFile << "P6\n" << newWidth << " " << newHeight << "\n" << 255 << "\n"; // Para Pixel8
        const auto& dstPixels = std::get<std::vector<Pixel8>>(dstImage.pixels);
        for (const auto& pixel : dstPixels) {
            outFile.write(reinterpret_cast<const char*>(&pixel.r), 1);
            outFile.write(reinterpret_cast<const char*>(&pixel.g), 1);
            outFile.write(reinterpret_cast<const char*>(&pixel.b), 1);
        }
    } else if (std::holds_alternative<std::vector<Pixel16>>(dstImage.pixels)) {
        outFile << "P6\n" << newWidth << " " << newHeight << "\n" << 65535 << "\n"; // Para Pixel16
        const auto& dstPixels = std::get<std::vector<Pixel16>>(dstImage.pixels);
        for (const auto& pixel : dstPixels) {
            outFile.write(reinterpret_cast<const char*>(&pixel.r), sizeof(pixel.r));
            outFile.write(reinterpret_cast<const char*>(&pixel.g), sizeof(pixel.g));
            outFile.write(reinterpret_cast<const char*>(&pixel.b), sizeof(pixel.b));
        }
    }

    outFile.close();
    std::cout << "La imagen con el nuevo nivel máximo de intensidad se ha guardado en outputresize.ppm." << std::endl;
    //
    return dstImage;
}
