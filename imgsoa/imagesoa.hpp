//
// Created by mapor on 26/10/2024.
//

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include <vector>
#include <cstdint>
#include <variant>
#include "./common/binaryio.hpp"
// Estructura que representa la imagen en formato SOA (Structure of Arrays)
struct ImageSOA {
  std::variant<std::vector<uint8_t>, std::vector<uint16_t>> redChannel; // Vector de píxeles (estructura SOA)
  std::variant<std::vector<uint8_t>, std::vector<uint16_t>> greenChannel; // Vector de píxeles (estructura SOA)
  std::variant<std::vector<uint8_t>, std::vector<uint16_t>> blueChannel; // Vector de píxeles (estructura SOA)
};
constexpr int METATADATA_MAX_VALUE = 255;
constexpr int BYTE_SIZE = 8;

ImageSOA cargarImagenPPMSOA(const std::string& nombre_archivo, PPMMetadata& metadata);

void imprimirImagenSOA(const ImageSOA& imagen, const PPMMetadata& metadata);

#endif //IMAGESOA_HPP
