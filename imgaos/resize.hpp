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

uint8_t interpolate8(uint8_t v00, uint8_t v01, float ttt);
uint16_t interpolate16(uint16_t v00, uint16_t v01, float ttt);
Pixel16 interpolatePixel(const Pixel16& p00, const Pixel16& p01, float ttt);
Pixel8 interpolatePixel(const Pixel8& p00, const Pixel8& p01, float ttt);

template<typename T>
T getPixel(const std::vector<T>& pixels, size_t width, size_t hor, size_t ver);
template<typename T>
std::vector<T> resizePixels(const std::vector<T>& srcPixels, const PPMMetadata& metadata, size_t newWidth, size_t newHeight);
// Declaración de la función
ImageAOS resize(const ImageAOS& srcImage, const PPMMetadata& metadata, const std::vector<size_t>& newSize, const std::string& OutputPath);
#endif //RESIZE_HPP
