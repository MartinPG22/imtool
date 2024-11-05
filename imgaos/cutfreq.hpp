//
// Created by mapor on 29/10/2024.
//

#ifndef CUTFREQ_AOS_HPP
#define CUTFREQ_AOS_HPP

#include "imageaos.hpp"
#include "compress.hpp"
#include "./common/binaryio.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <variant>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <limits>
#include <sstream>  // Incluimos <sstream> para std::istringstream

/*
void cutfreq(ImageAOS& srcImage, const PPMMetadata& metadata, int nColores, const std::string& outputPath);
template <typename T>
constexpr Color<T> extractRGB48(u_int64_t rgb);
template <typename T>
constexpr Color<T> extractRGB8(int rgb);
template <typename T>
constexpr int combineRGB(const Color<T>& color);

//Pruebas
void countColorFrequency(const ImageAOS& srcImage, std::unordered_map<int, int>& colorFrequency);
void sortColorsByFrequency(std::unordered_map<int, int>& colorFrequency, std::vector<std::tuple<int, Color<int>>>& colorData, int maxLevel);
struct ColorSplit {
  std::vector<std::tuple<int, Color<int>>> colorsToRemove;
  std::vector<std::tuple<int, Color<int>>> colorsToKeep;
};

// Dividir colores en los que se mantienen y los que se eliminan
ColorSplit splitColors(const std::vector<std::tuple<int, Color<int>>>& colorData, int nColores);
struct ColorGroups {
  std::vector<std::tuple<int, Color<int>>> colorsToRemove;
  std::vector<std::tuple<int, Color<int>>> colorsToKeep;
};

Color<int> findClosestColor(const Color<int>& colorRem, const std::vector<std::tuple<int, Color<int>>>& colorsToKeep);
void createReplacementMap(const ColorGroups& colorGroups,
                          std::unordered_map<int, Color<int>>& replacementMap);
void applyColorReplacement(ImageAOS& srcImage, const std::unordered_map<int, Color<int>>& replacementMap);*/
void cutfreq(ImageAOS& srcImage, const PPMMetadata& metadata, int nColores, const std::string& outputPath);
template <typename T>
constexpr Color<T> extractRGB48(u_int64_t rgb);
template <typename T>
constexpr Color<T> extractRGB8(u_int64_t rgb);
template <typename T>
constexpr uint64_t combineRGB(const Color<T>& color);
template <typename T>
constexpr uint64_t combineRGB48(const Color<T>& color);

//Pruebas
void countColorFrequency(const ImageAOS& srcImage, std::unordered_map<u_int64_t, int>& colorFrequency);
void sortColorsByFrequency(std::unordered_map<u_int64_t, int>& colorFrequency, std::vector<std::tuple<int, Color<int>>>& colorData, int maxLevel);
struct ColorSplit {
  std::vector<std::tuple<int, Color<int>>> colorsToRemove;
  std::vector<std::tuple<int, Color<int>>> colorsToKeep;
};

// Dividir colores en los que se mantienen y los que se eliminan
ColorSplit splitColors(const std::vector<std::tuple<int, Color<int>>>& colorData, int nColores);
struct ColorGroups {
  std::vector<std::tuple<int, Color<int>>> colorsToRemove;
  std::vector<std::tuple<int, Color<int>>> colorsToKeep;
};

Color<int> findClosestColor(const Color<int>& colorRem, const std::vector<std::tuple<int, Color<int>>>& colorsToKeep);
void createReplacementMap(const ColorGroups& colorGroups,
                          std::unordered_map<int, Color<int>>& replacementMap);
void applyColorReplacement(ImageAOS& srcImage, const std::unordered_map<int, Color<int>>& replacementMap);
#endif //CUTFREQ_AOS_HPP
