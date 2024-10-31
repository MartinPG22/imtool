//
// Created by asanch on 10/19/24.
//

#ifndef MAXLEVEL_HPP
# define MAXLEVEL_HPP

#include "imageaos.hpp"

int maxlevel(const ImageAOS& srcImage,
    const PPMMetadata& metadata, int newMaxLevel, const std::string& outputPath);

#endif //MAXLEVEL_HPP