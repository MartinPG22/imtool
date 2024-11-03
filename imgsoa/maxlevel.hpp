//
// Created by asanch on 11/2/24.
//

#ifndef MAXLEVEL_SOA_HPP
#define MAXLEVEL_SOA_HPP

#include <iostream>
#include "imagesoa.hpp"

template <typename PixelType>
int maxlevelSOA(const ImageSOA<PixelType>& srcImage, const PPMMetadata& metadata, int newMaxLevel, const std::string& outputPath);

#endif //MAXLEVEL_SOA_HPP
