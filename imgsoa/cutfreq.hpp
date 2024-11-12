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

// Function declarations matching cutfreq.cpp
void countColorFrequency(const ImageSOA& srcImage, std::unordered_map<int, int>& colorFrequency);
void applyColorReplacement(ImageSOA& srcImage, const std::unordered_map<int, std::tuple<int, int, int>>& replacementMap);
void cutfreqSOA(ImageSOA& srcImage, const PPMMetadata& metadata, int nColores, const std::string& outputPath);
void saveImageSOAToPPM(const ImageSOA& image, const PPMMetadata& metadata, const std::string& outputPath);
// Combina los valores RGB en una única clave entera
constexpr int combineRGB(int red, int green, int blue) {
  return (red << changeBits16) | (green << BYTE_SIZE) | blue;
}
std::vector<std::tuple<int, int, int, int>> getColorData(const std::unordered_map<int, int>& colorFrequency);
std::tuple<int, int, int> findClosestReplacement(const std::tuple<int, int, int>& targetColor,
                                                 const std::vector<std::tuple<int, int, int, int>>& colorData,
                                                 size_t startIdx);
std::unordered_map<int, std::tuple<int, int, int>> createReplacementMap(
    const std::vector<std::tuple<int, int, int, int>>& colorData, int nColores);
void applyReplacementsAndSave(ImageSOA& srcImage,
                              const std::unordered_map<int, std::tuple<int, int, int>>& replacementMap,
                              const PPMMetadata& metadata, const std::string& outputPath);

#endif // CUTFREQ_SOA_HPP