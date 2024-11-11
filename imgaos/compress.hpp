//
// Created by mapor on 26/10/2024.
//

#ifndef COMPRESS_AOS_HPP
#define COMPRESS_AOS_HPP
#include "imageaos.hpp"
#include "./common/binaryio.hpp"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include <cstdint>
#include <set>
template <typename T>
struct Color {
  std::array<T, 3> components; // Usar un array para almacenar los componentes

  // Constructor predeterminado
  Color() : components{0, 0, 0} {}  // Inicializa RGB a 0

  // Constructor
  Color(T red, T green, T blue) : components{red, green, blue} {}

  // Accesores para facilitar el acceso a los componentes
  [[nodiscard]] T r() const { return components[0]; }
  [[nodiscard]] T g() const { return components[1]; }
  [[nodiscard]] T b() const { return components[2]; }
};

void writeCPPM(const ImageAOS& image, const std::string& filename, const PPMMetadata& metadata) ;
size_t writeHeader(std::ofstream& file, const PPMMetadata& metadata, size_t colorCount);
void finish(std::vector<uint8_t> buffer, std::ofstream& file, const std::string& filename);
bool operator==(const Pixel8& lhs, const Pixel8& rhs);
bool operator==(const Pixel16& lhs, const Pixel16& rhs);

#endif //COMPRESS_AOS_HPP
