//
// Created by mapor on 19/10/2024.
//

#ifndef BINARYIO_HPP
#define BINARYIO_HPP

#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <stdexcept>

// Estructura para almacenar los metadatos de la imagen
struct PPMMetadata {
  size_t width;
  size_t height;
  int max_value;
};
// Prototipo de la función para leer un archivo binario
std::vector<uint8_t> readBinaryFile(const std::string& filename);

#endif // BINARYIO_HPP
