
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


// Función principal para redimensionar la imagen
ImageAOS resize(const ImageAOS& srcImage, const PPMMetadata& metadata, const std::vector<size_t>& newSize, const std::string& outputPath) {
    ImageAOS dstImage;
    size_t const newWidth = newSize[0];
    size_t const newHeight = newSize[1];
    if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels)) {
        const auto& srcPixels = std::get<std::vector<Pixel8>>(srcImage.pixels);
        dstImage.pixels = resizePixels(srcPixels, metadata, newWidth, newHeight);

    } else if (std::holds_alternative<std::vector<Pixel16>>(srcImage.pixels)) {
        const auto& srcPixels = std::get<std::vector<Pixel16>>(srcImage.pixels);
        dstImage.pixels = resizePixels(srcPixels, metadata, newWidth, newHeight);
    }
    // Tengo que crear un nuevo metadata con las nuevas dimensiones
    PPMMetadata newMetadata{};
    newMetadata.width = newWidth;
    newMetadata.height = newHeight;

    saveAOStoPPM(dstImage, newMetadata, metadata.max_value, outputPath);
    std::cout << "La imagen con el nuevo tamaño se ha guardado en " << outputPath << '\n';
    return dstImage;
}

