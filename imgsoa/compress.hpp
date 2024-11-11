//
// Created by mapor on 29/10/2024.
//

#ifndef COMPRESS_SOA_HPP
#define COMPRESS_SOA_HPP

#include "imagesoa.hpp"
#include "cutfreq.hpp"
#include "./common/binaryio.hpp"
#include <iostream>
#include <fstream>
#include <variant>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <cstdint>
#include <array>  // Incluir la biblioteca para std::array
#include <set>

struct TupleHash {
  template <typename... T>
  std::size_t operator()(const std::tuple<T...>& tuple) const {
    return std::apply([](const T&... args) {
        return (... ^ std::hash<T>{}(args)); // Usar T en lugar de decltype(args)
    }, tuple);
  }
};
struct PixelData {
  std::vector<uint8_t> redChannel;
  std::vector<uint8_t> greenChannel;
  std::vector<uint8_t> blueChannel;
  std::unordered_map<std::tuple<int, int, int>, uint32_t, TupleHash> colorTable;
};
void writeCPPMSOA(const ImageSOA& image, const std::string& filename, const PPMMetadata& metadata);
size_t determinePixelSize(size_t colorCount);
//void writeBytes(std::ofstream& file, const std::vector<char>& buffer);
void writeBytes(std::ofstream& file, const std::vector<uint8_t>& buffer);
void writeColorTable(std::ofstream& file, const std::vector<std::tuple<int, int, int>>& colorList, size_t colorBytes);
void writePixelData(std::ofstream& file, const PixelData& pixelData, size_t pixelSize);
PixelData prepareColorChannels(const ImageSOA& image, std::unordered_map<std::tuple<int, int, int>, uint32_t, TupleHash>& colorTable);
#endif //COMPRESS_SOA_HPP
