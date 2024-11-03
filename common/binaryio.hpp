//
// Created by mapor on 19/10/2024.
//

#ifndef BINARYIO_HPP
#define BINARYIO_HPP

#include <fstream>
#include <vector>
#include <cstdint>
#include <string>


constexpr int MAX_16 = 65535;
constexpr int changeBits16 = 16;
constexpr uint8_t MAX_PIXEL_VALUE = 0xFF; // Valor máximo para píxeles de 8 bits
constexpr int METATADATA_MAX_VALUE = 255;
constexpr int BYTE_SIZE = 8;

constexpr int MAX_8 = 255;
constexpr int changeBits = 8;

constexpr int buffer_size_8 = 3;
constexpr int buffer_size_16 = 6;
constexpr int MAX_COLOR_VALUE = 0xFF;
constexpr uint8_t MIN_PIXEL_VALUE = 8;

// Estructura para almacenar los metadatos de la imagen
struct PPMMetadata {
  size_t width;
  size_t height;
  int max_value;
};
// Prototipo de la función para leer un archivo binario
std::vector<uint8_t> readBinaryFile(const std::string& filename);
PPMMetadata readPPMMetadata(const std::string& filename);
std::string readLine(const std::vector<uint8_t>& data, size_t& index);
int readNextInt(const std::vector<uint8_t>& data, size_t& index, const std::string& errorMsg);
#endif // BINARYIO_HPP
