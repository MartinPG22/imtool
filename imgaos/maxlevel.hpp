//
// Created by asanch on 10/19/24.
//

#ifndef MAXLEVEL_AOS_HPP
# define MAXLEVEL_AOS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>

#include "imageaos.hpp"

ImageAOS maxlevelAOS(const ImageAOS &srcImage, const PPMMetadata &metadata, int newMaxLevel,
                     const std::string &outputPath);

#endif //MAXLEVEL_AOS_HPP
