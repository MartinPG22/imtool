//
// Created by mapor on 26/10/2024.
//

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include <vector>
#include <cstdint>
#include <variant>
#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include "./common/binaryio.hpp"


constexpr int METATADATA_MAX_VALUE = 255;
constexpr int BYTE_SIZE = 8;

// Estructura que representa la imagen en formato SOA (Structure of Arrays)
// PixelType es el tipo de dato de los píxeles (uint8_t o uint16_t)
template <typename PixelType>
struct ImageSOA {
    std::vector<PixelType> redChannel; // Vector de píxeles (estructura SOA)
    std::vector<PixelType> greenChannel; // Vector de píxeles (estructura SOA)
    std::vector<PixelType> blueChannel; // Vector de píxeles (estructura SOA)
};

template <typename PixelType>
ImageSOA<PixelType> cargarImagenPPMSOA(const std::string& nombre_archivo, PPMMetadata& metadata);
template <typename PixelType>
int saveSOAtoPPM(const ImageSOA<PixelType>& srcImage, const PPMMetadata& metadata, int maxLevel, const std::string& outputPath);
template <typename PixelType>
void imprimirImagenSOA(const ImageSOA<PixelType>& imagen, const PPMMetadata& metadata);

#endif //IMAGESOA_HPP
