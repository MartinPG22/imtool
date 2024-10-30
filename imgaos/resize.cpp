/*
#include <iostream>  // Agregado para std::cout y std::cerr
#include <variant>
#include <vector>
#include <cmath>

#include <fstream>   // Agregado para std::ofstream
#include "resize.hpp"
#include "imageaos.hpp"
#include "./common/binaryio.hpp"


// Función de interpolación lineal entre dos valores
template<typename T>
T interpolate(T v00, T v01, float t) {
    return static_cast<T>((v00 * (1 - t)) + (v01 * t));
}
// Interpolación de un píxel de 8 bits
Pixel8 interpolatePixel(const Pixel8& p00, const Pixel8& p01, float t) {
    Pixel8 result;
    result.r = interpolate(p00.r, p01.r, t);
    result.g = interpolate(p00.g, p01.g, t);
    result.b = interpolate(p00.b, p01.b, t);
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
ImageAOS resize(const ImageAOS& srcImage, const PPMMetadata& metadata, size_t newWidth, size_t newHeight, const std::string& OutputPath) {
    ImageAOS dstImage;

    if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels)) {
        const auto& srcPixels = std::get<std::vector<Pixel8>>(srcImage.pixels);
        dstImage.pixels = resizePixels(srcPixels, metadata, newWidth, newHeight);
        //imprimirPixeles(dstImage);
    } else if (std::holds_alternative<std::vector<Pixel16>>(srcImage.pixels)) {
        const auto& srcPixels = std::get<std::vector<Pixel16>>(srcImage.pixels);
        dstImage.pixels = resizePixels(srcPixels, metadata, newWidth, newHeight);
    }

    // Guardar el vector de píxeles en un nuevo archivo PPM
    std::ofstream outFile(OutputPath, std::ios::binary);
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

            // Imprimir los resultados de las 3 primeras iteraciones
            static int count = 0; // Contador estático para mantener el estado entre iteraciones
            if (count < 10) {
                std::cout << "Pixel " << count + 1 << ": ("
                          << reinterpret_cast<const char*>(&pixel.r) << ", "
                          << reinterpret_cast<const char*>(&pixel.g) << ", "
                          << reinterpret_cast<const char*>(&pixel.b) << ")\n";
                count++; // Incrementar el contador
            }
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
    std::cout << "La imagen con el nuevo nivel máximo de intensidad se ha guardado en el OutputPath" << std::endl;
    //
    return dstImage;
}
*/

#include "resize.hpp"


// Interpolación para valores de 8 bits
uint8_t interpolate8(uint8_t v00, uint8_t v01, float ttt) {
    return static_cast<uint8_t>((static_cast<float>(v00) * (1.0F - ttt)) + (static_cast<float>(v01) * ttt));
}

// Interpolación para valores de 16 bits
uint16_t interpolate16(uint16_t v00, uint16_t v01, float ttt) {
    return static_cast<uint16_t>((static_cast<float>(v00) * (1.0F - ttt)) + (static_cast<float>(v01) * ttt));
}

// Interpolación de un píxel de 8 bits
Pixel8 interpolatePixel(const Pixel8& p00, const Pixel8& p01, float ttt) {
    Pixel8 result{};
    result.r = interpolate8(p00.r, p01.r, ttt);
    result.g = interpolate8(p00.g, p01.g, ttt);
    result.b = interpolate8(p00.b, p01.b, ttt);
    return result;
}

    // Interpolación de un píxel de 16 bits
Pixel16 interpolatePixel(const Pixel16& p00, const Pixel16& p01, float ttt) {
    Pixel16 result{};
    result.r = interpolate16(p00.r, p01.r, ttt);
    result.g = interpolate16(p00.g, p01.g, ttt);
    result.b = interpolate16(p00.b, p01.b, ttt);
    return result;
}

    // Función para guardar una imagen PPM de 8 bits
void savePixelsToPPM8(const std::string& outputPath, const std::vector<Pixel8>& pixels, const std::vector<size_t>& newSize, size_t intensidad ) {
    size_t const newWidth = newSize[0];
    size_t const newHeight = newSize[1];
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo de salida" << '\n';
        return; // Salir si no se puede abrir el archivo
    }// Puede que este mal la intensidad del encabezado
    outFile << "P6\n" << newWidth << " " << newHeight << "\n" << intensidad << "\n"; // Encabezado para Pixel8
    for (const auto& pixel : pixels) {
        outFile.put(static_cast<char>(pixel.r));
        outFile.put(static_cast<char>(pixel.g));
        outFile.put(static_cast<char>(pixel.b));
    }
    outFile.close();
}

// Función para guardar una imagen PPM de 16 bits
void savePixelsToPPM16(const std::string& outputPath, const std::vector<Pixel16>& pixels, const std::vector<size_t>& newSize, size_t intensidad ) {
    size_t const newWidth = newSize[0];
    size_t const newHeight = newSize[1];
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo de salida" << '\n';
        return; // Salir si no se puede abrir el archivo
    }
    outFile << "P6\n" << newWidth << " " << newHeight << "\n" << intensidad << "\n"; // Encabezado para Pixel16
    for (const auto& pixel : pixels) {
        outFile.put(static_cast<char>(pixel.r >> changeBits)); // Escribir el byte alto
        outFile.put(static_cast<char>(pixel.r & MAX_PIXEL_VALUE)); // Escribir el byte bajo
        outFile.put(static_cast<char>(pixel.g >> changeBits)); // Escribir el byte alto
        outFile.put(static_cast<char>(pixel.g & MAX_PIXEL_VALUE)); // Escribir el byte bajo
        outFile.put(static_cast<char>(pixel.b >> changeBits)); // Escribir el byte alto
        outFile.put(static_cast<char>(pixel.b & MAX_PIXEL_VALUE)); // Escribir el byte bajo
    }
    outFile.close();
}


// Función principal para redimensionar la imagen
ImageAOS resize(const ImageAOS& srcImage, const PPMMetadata& metadata, const std::vector<size_t>& newSize, const std::string& outputPath) {
    ImageAOS dstImage;
    size_t const newWidth = newSize[0];
    size_t const newHeight = newSize[1];
    auto const intensidad = static_cast<size_t>(metadata.max_value);
    if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels)) {
        const auto& srcPixels = std::get<std::vector<Pixel8>>(srcImage.pixels);
        dstImage.pixels = resizePixels(srcPixels, metadata, newWidth, newHeight);
        savePixelsToPPM8(outputPath, std::get<std::vector<Pixel8>>(dstImage.pixels), newSize, intensidad);
    } else if (std::holds_alternative<std::vector<Pixel16>>(srcImage.pixels)) {
        const auto& srcPixels = std::get<std::vector<Pixel16>>(srcImage.pixels);
        dstImage.pixels = resizePixels(srcPixels, metadata, newWidth, newHeight);
        savePixelsToPPM16(outputPath, std::get<std::vector<Pixel16>>(dstImage.pixels), newSize, intensidad);
    }

    std::cout << "La imagen con el nuevo nivel máximo de intensidad se ha guardado en " << outputPath << '\n';
    return dstImage;
}