//
// Created by mapor on 07/10/2024.
//

#include "binaryio.hpp"
#include <vector>
#include <cstdint>
#include <string>
#include <stdexcept>

#include "binaryio.hpp"

// Función para leer un archivo binario y devolver un vector de bytes
std::vector<uint8_t> readBinaryFile(const std::string& filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("No se pudo abrir el archivo binario.");
  }

  // Obtener el tamaño del archivo
  file.seekg(0, std::ios::end);
  std::streamoff fileSize = file.tellg(); // Obtiene la posición del final del archivo

  // Verificar si tellg() tuvo éxito
  if (fileSize == -1) {
    throw std::runtime_error("Error al determinar el tamaño del archivo.");
  }

  // Asegurarse de que fileSize no sea negativo antes de convertir
  if (fileSize < 0) {
    throw std::runtime_error("El tamaño del archivo es negativo.");
  }

  file.seekg(0, std::ios::beg);
  // Usar static_cast para evitar la advertencia de conversión de signación
  std::vector<uint8_t> buffer(static_cast<size_t>(fileSize)); // Conversión explícita
  file.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(fileSize));

  file.close();
  return buffer;
}

// Función para leer los metadatos de un archivo PPM
PPMMetadata readPPMMetadata(const std::string& filename) {
  std::vector<uint8_t> fileData = readBinaryFile(filename);
  size_t index = 0;
  PPMMetadata metadata;

  // Leer el formato PPM
  std::string format;
  while (fileData[index] != '\n') {
    format += static_cast<char>(fileData[index++]); // Conversión explícita
  }
  ++index;  // Saltar el '\n'

  if (format != "P6") {
    throw std::runtime_error("Formato PPM no soportado.");
  }

  // Leer ancho, alto y valor máximo
  auto readNextInt = [&fileData, &index]() -> int {
    std::string num;
    while (fileData[index] == ' ' || fileData[index] == '\n') {
      ++index;  // Saltar espacios y saltos de línea
    }
    while (fileData[index] >= '0' && fileData[index] <= '9') {
      num += static_cast<char>(fileData[index++]);

    }
    return std::stoi(num);
  };

  int widthValue = readNextInt();
  if (widthValue < 0) {
    throw std::runtime_error("Ancho de imagen no válido.");
  }
  metadata.width = static_cast<size_t>(widthValue);

  int heightValue = readNextInt();
  if (heightValue < 0) {
    throw std::runtime_error("Altura de imagen no válida.");
  }
  metadata.height = static_cast<size_t>(heightValue);

  metadata.max_value = readNextInt();

  // Asegurarse de que max_value esté en el rango esperado
  if (metadata.max_value > 255 && metadata.max_value != 65535) {
    throw std::runtime_error("Máximo valor de color no soportado.");
  }

  return metadata;
}
//df