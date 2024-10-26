//
// Created by mapor on 26/10/2024.
//

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include <vector>
#include <cstdint>
#include <variant>
// Estructura que representa la imagen en formato AOS (Array of Structures)
struct ImageSOA {
  std::variant<std::vector<uint8_t>, std::vector<uint16_t>> redChannel; // Vector de píxeles (estructura AOS)
  std::variant<std::vector<uint8_t>, std::vector<uint16_t>> greenChannel; // Vector de píxeles (estructura AOS)
  std::variant<std::vector<uint8_t>, std::vector<uint16_t>> blueChannel; // Vector de píxeles (estructura AOS)
};

#endif //IMAGESOA_HPP
