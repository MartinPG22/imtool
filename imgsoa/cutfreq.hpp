//
// Created by mapor on 31/10/2024.
//

#ifndef CUTFREQAOS_HPP
#define CUTFREQAOS_HPP

#include <iostream>
#include <fstream>
#include <variant>
#include <vector>
#include <string>
#include "imagesoa.hpp"
#include "./common/binaryio.hpp"
struct ColorFrequencies {
  int redCount = 0;
  int greenCount = 0;
  int blueCount = 0;
};
ColorFrequencies contarFrecuencias(const ImageSOA& imagen, int width, int height);
void cutfreqAOS(const ImageSOA& imagen, const PPMMetadata& metadata, const std::string& outputPath, const ColorFrequencies& freqs );
void saveImageSOAToPPM(const ImageSOA& image, const PPMMetadata& metadata, const std::string& outputPath);
#endif //CUTFREQAOS_HPP
