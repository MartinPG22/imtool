#ifndef RESIZESOA_HPP
#define RESIZESOA_HPP

#include <vector>
#include "imagesoa.hpp" // Asegúrate de que este archivo existe y es accesible
#include "./common/binaryio.hpp" // Asegúrate de que este archivo existe y es accesible

// Estructura para agrupar los canales de color de 8 bits
struct ColorChannels8Bit {
  std::vector<uint8_t> red;
  std::vector<uint8_t> green;
  std::vector<uint8_t> blue;
};

// Estructura para agrupar los canales de color de 16 bits
struct ColorChannels16Bit {
  std::vector<uint16_t> red;
  std::vector<uint16_t> green;
  std::vector<uint16_t> blue;
};

// Definir constantes para el valor de 8 bits y la cantidad de desplazamiento
static constexpr uint8_t MAX_PIXEL_VALUE_8BIT = 0xFF;
static constexpr uint8_t BITS_TO_SHIFT = 8;  // Cantidad de bits a desplazar para obtener el byte alto

uint8_t interpolate8SOA(uint8_t v00, uint8_t v01, float ttt);
uint16_t interpolate16SOA(uint16_t v00, uint16_t v01, float ttt);

template<typename T>
T interpolatePixelSOA(T v00, T v01, float ttt);

template<typename T>
T getPixelSOA(const std::vector<T>& pixels, size_t width, size_t hor, size_t ver);

template<typename T>
std::vector<T> resizePixelsSOA(const std::vector<T>& srcPixels, const PPMMetadata& metadata, size_t newWidth, size_t newHeight);

ImageSOA resizeSOA(const ImageSOA& srcImage, const PPMMetadata& metadata, const std::vector<size_t>& newSize, const std::string& outputPath);

#endif // RESIZESOA_HPP

