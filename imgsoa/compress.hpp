//
// Created by mapor on 29/10/2024.
//

#ifndef COMPRESSAOS_HPP
#define COMPRESSAOS_HPP

#include "imagesoa.hpp"
#include "./common/binaryio.hpp"
#include <iostream>
#include <fstream>
#include <variant>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <cstdint>
#include <array>  // Incluir la biblioteca para std::array

struct TupleHash {
  template <typename... T>
  std::size_t operator()(const std::tuple<T...>& tuple) const {
    return std::apply([](const T&... args) {
        return (... ^ std::hash<T>{}(args)); // Usar T en lugar de decltype(args)
    }, tuple);
  }
};


void writeCPPMSOA(const ImageSOA& image, const std::string& filename, const PPMMetadata& metadata);

#endif //COMPRESS_HPPAOS
