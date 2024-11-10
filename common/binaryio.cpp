//
// Created by mapor on 07/10/2024.
//

#include "binaryio.hpp"

// Función para leer un archivo binario y devolver un vector de bytes
std::vector<uint8_t> readBinaryFile(const std::string& filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("No se pudo abrir el archivo binario.");
  }

  // Obtener el tamaño del archivo
  file.seekg(0, std::ios::end);
  const std::streamoff fileSize = file.tellg(); // Obtiene la posición del final del archivo

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
  std::vector<char> buffer(static_cast<size_t>(fileSize));
  file.read(buffer.data(), static_cast<std::streamsize>(fileSize));

  file.close();
  // Convertir buffer de char a uint8_t
  std::vector<uint8_t> uint8Buffer(buffer.begin(), buffer.end());
  return uint8Buffer;
}


// Función auxiliar para leer una línea completa (hasta '\n') y convertirla en string
std::string readLine(const std::vector<uint8_t>& data, size_t& index) {
  std::string line;
  while (index < data.size() && data[index] != '\n') {
    line += static_cast<char>(data[index++]);
  }
  ++index;  // Saltar el '\n'
  return line;
}

// Función auxiliar para leer el siguiente número entero en el archivo
int readNextInt(const std::vector<uint8_t>& data, size_t& index, const std::string& errorMsg) {
  // Saltar espacios y saltos de línea
  while (index < data.size() && (data[index] == ' ' || data[index] == '\n')) {
    ++index;
  }
  std::string num;
  while (index < data.size() && data[index] >= '0' && data[index] <= '9') {
    num += static_cast<char>(data[index++]);
  }

  int const value = std::stoi(num);
  if (value < 0) {
    throw std::runtime_error(errorMsg);
  }
  return value;
}

// Función para leer los metadatos de un archivo PPM
PPMMetadata readPPMMetadata(const std::string& filename) {
  std::vector<uint8_t> const fileData = readBinaryFile(filename);
  size_t index = 0;
  PPMMetadata metadata{};

  // Leer y verificar el formato PPM (P6)
  std::string const format = readLine(fileData, index);
  if (format != "P6") {
    throw std::runtime_error("Formato PPM no soportado.");
  }

  // Leer ancho, alto y valor máximo
  metadata.width = static_cast<size_t>(readNextInt(fileData, index, "Ancho de imagen no válido."));
  metadata.height = static_cast<size_t>(readNextInt(fileData, index, "Altura de imagen no válida."));
  metadata.max_value = readNextInt(fileData, index, "Máximo valor de color no válido.");

  // Verificar el rango de max_value (1 a 65535)
  if (metadata.max_value < 1 || metadata.max_value > MAX_16) {
    throw std::runtime_error("Máximo valor de color fuera de rango. Debe estar entre 1 y 65535.");
  }

  return metadata;
}


