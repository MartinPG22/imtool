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
#include <unordered_map>
#include <tuple>
#include "imagesoa.hpp"
#include "./common/binaryio.hpp"

constexpr int MAX_COLOR_VALUE = 0xFF;

// Function declarations matching cutfreq.cpp
void countColorFrequency(const ImageSOA& srcImage, std::unordered_map<int, int>& colorFrequency);
void applyColorReplacement(ImageSOA& srcImage, const std::unordered_map<int, std::tuple<int, int, int>>& replacementMap);
void cutfreqSOA(ImageSOA& srcImage, const PPMMetadata& metadata, int nColores, const std::string& outputPath);
void saveImageSOAToPPM(const ImageSOA& image, const PPMMetadata& metadata, const std::string& outputPath);

#endif // CUTFREQ_SOA_HPP