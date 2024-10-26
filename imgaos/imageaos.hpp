//
// Created by mapor on 19/10/2024.
//

#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP


#include <vector>
#include <cstdint>
#include <variant>

/// Estructura que representa un píxel con componentes R, G, B
struct Pixel16 {
  uint16_t r, g, b; // Valores de 0-65535
};

struct Pixel8 {
  uint8_t r, g, b; // Valores de 0-255
};
// A lo mejor las estructuras de pixel pueden ir en common

// Estructura que representa la imagen en formato AOS (Array of Structures)
struct ImageAOS {
  std::variant<std::vector<Pixel8>, std::vector<Pixel16>> pixels; // Vector de píxeles (estructura AOS)
};


#endif //IMAGESOA_HPP
