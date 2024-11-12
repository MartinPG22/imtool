//
// Created by mapor on 14/10/2024.
//

#ifndef PROGARGS_HPP
#define PROGARGS_HPP

#include "binaryio.hpp"

#include "../imgaos/imageaos.hpp"
#include "../imgaos/maxlevel.hpp"
#include "../imgaos/resize.hpp"
#include "../imgaos/compress.hpp"
#include "../imgaos/cutfreq.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <filesystem>  // Para trabajar con rutas de archivo

#include "../imgsoa/imagesoa.hpp"
#include "../imgsoa/maxlevel.hpp"
#include "../imgsoa/cutfreq.hpp"
#include "../imgsoa/compress.hpp"
#include "../imgsoa/resize.hpp"

constexpr size_t ARG_RESIZE = 5;
constexpr int MAX_NEW_LEVEL = 65535;
constexpr size_t MIN_NEW_LEVEL = 1;
bool isInteger(const std::string& integer);


int executeOperation(const std::vector<std::string>& arguments,const std::string& method);
int executeInfo(const std::vector<std::string>& arguments, const PPMMetadata& metadata);
int executeMaxlevel(const std::vector<std::string>& arguments, PPMMetadata& metadata, const std::string& method);
int executeResize(const std::vector<std::string>& arguments, PPMMetadata& metadata, const std::string& method);
int executeCutfreq(const std::vector<std::string>& arguments, PPMMetadata& metadata, const std::string& method);
int executeCompress(const std::vector<std::string>& arguments, PPMMetadata& metadata, const std::string& method);
int argumentsResize(const std::vector<std::string>& arguments);


#endif //PROGARGS_HPP
