//
// Created by mapor on 29/10/2024.
//

#ifndef CUTFREQ_HPP
#define CUTFREQ_HPP

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
#include <cmath> // Para std::sqrt
#include <limits> // Para std::numeric_limits
/*
double distanciaEuclidiana(const Pixel8& first, const Pixel8& second);
ImageAOS cutfreq(const ImageAOS& srcImage, const PPMMetadata& metadata, int nColores, const std::string& outputPath);
void saveImage(const ImageAOS& image, const PPMMetadata& metadata, const std::string& outputPath);
*/
void cutfreq(ImageAOS& srcImage, const PPMMetadata& metadata, int nColores, const std::string& outputPath);
//constexpr int combineRGB(int red, int green, int blue);
//constexpr void extractRGB(int rgb, int& red, int& green, int& blue);
void saveToPPM(const ImageAOS& srcImage, const PPMMetadata& metadata, const std::string& outputPath);
template <typename T>
constexpr Color<T> extractRGB(int rgb);
template <typename T>
constexpr int combineRGB(const Color<T>& color);
#endif //CUTFREQ_HPP
