//
// Created by mapor on 19/10/2024.
//

#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include <vector>
#include <cstdint>
#include <variant>
#include <iostream>
#include <string>
#include <array>  // Asegúrate de incluir este encabezado
#include "./common/binaryio.hpp"



// Estructura que representa un píxel con componentes R, G, B
struct Pixel16 {
  uint16_t r, g, b;}; // Valores de 0-65535
struct Pixel8 {
  uint8_t r, g, b;}; // Valores de 0-255
// Estructura que representa la imagen en formato AOS (Array of Structures)
struct ImageAOS {
  std::variant<std::vector<Pixel8>, std::vector<Pixel16>> pixels; // Vector de píxeles (estructura AOS)
};
ImageAOS cargarPixels8(std::ifstream& archivo, size_t num_pixels);
ImageAOS cargarPixels16(std::ifstream& archivo, size_t num_pixels);
ImageAOS cargarImagenPPMAOS(const std::string& nombre_archivo, PPMMetadata& metadata);
int saveAOStoPPM(const ImageAOS& srcImage, const PPMMetadata& metadata, int maxLevel, const std::string& outputPath);

#endif //IMAGEAOS_HPP
