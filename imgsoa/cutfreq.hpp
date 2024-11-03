//
// Created by mapor on 31/10/2024.
//

#ifndef CUTFREQ_SOA_HPP
#define CUTFREQ_SOA_HPP

#include <iostream>
#include <fstream>
#include <variant>
#include <vector>
#include <algorithm>
#include <string>
#include "imagesoa.hpp"
#include "./common/binaryio.hpp"

struct ColorFrequencies {
  int redCount = 0;
  int greenCount = 0;
  int blueCount = 0;
};
constexpr int HEX_VAL = 0xFF;

template <typename PixelType>
ColorFrequencies contarFrecuencias(const ImageSOA<PixelType> & imagen, int width, int height);
template <typename PixelType>
void cutfreqSOA(const ImageSOA<PixelType> & imagen, const PPMMetadata& metadata, const std::string& outputPath, const ColorFrequencies& freqs );
template <typename PixelType>
void saveImageSOAToPPM(const ImageSOA<PixelType> & image, const PPMMetadata& metadata, const std::string& outputPath);
#endif //CUTFREQ_SOA_HPP
