
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

