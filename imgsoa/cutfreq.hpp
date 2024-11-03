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
ColorFrequencies contarFrecuencias(const ImageSOA& imagen, int width, int height);
void cutfreqSOA(const ImageSOA& imagen, const PPMMetadata& metadata, const std::string& outputPath, const ColorFrequencies& freqs );
void saveImageSOAToPPM(const ImageSOA& image, const PPMMetadata& metadata, const std::string& outputPath);
#endif //CUTFREQ_SOA_HPP
