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
