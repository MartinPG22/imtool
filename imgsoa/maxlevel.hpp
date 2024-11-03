//
// Created by asanch on 11/2/24.
//

#ifndef MAXLEVEL_SOA_HPP
#define MAXLEVEL_SOA_HPP

#include <iostream>

#include "imagesoa.hpp"

int maxlevelSOA(const ImageSOA& srcImage, const PPMMetadata& metadata, int newMaxLevel, const std::string& outputPath);

#endif //MAXLEVEL_SOA_HPP
