//
// Created by asanch on 11/2/24.
//

#ifndef MAXLEVEL_SOA_HPP
#define MAXLEVEL_SOA_HPP

#include <iostream>
#include <algorithm>
#include<tuple>
#include "imagesoa.hpp"

ImageSOA maxlevelSOA(const ImageSOA& srcImage, const PPMMetadata& metadata, int newMaxLevel, const std::string& outputPath);

#endif //MAXLEVEL_SOA_HPP
