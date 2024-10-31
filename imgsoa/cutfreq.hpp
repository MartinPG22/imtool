//
// Created by mapor on 31/10/2024.
//

#ifndef CUTFREQAOS_HPP
#define CUTFREQAOS_HPP


#include "imagesoa.hpp"
#include "./common/binaryio.hpp"
struct ColorFrequencies {
  int redCount = 0;
  int greenCount = 0;
  int blueCount = 0;
};
//ColorFrequencies contarFrecuencias(const ImageSOA& imagen, int width, int height);
//void cutfreqAOS(ImageSOA& imagen, int width, const ColorFrequencies& freqs, int height);

#endif //CUTFREQAOS_HPP
