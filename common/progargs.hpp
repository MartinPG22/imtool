//
// Created by mapor on 14/10/2024.
//

#ifndef PROGARGS_HPP
#define PROGARGS_HPP

#include "binaryio.hpp"
#include "binaryio.cpp"
#include "./imgaos/imageaos.hpp"
#include "./imgaos/imageaos.cpp"
#include "./imgsoa/imagesoa.hpp"
#include "./imgsoa/imagesoa.cpp"

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <filesystem>  // Para trabajar con rutas de archivo

#include "../imgaos/maxlevel.cpp"
#include "../imgaos/resize.hpp"
#include "../imgaos/compress.cpp"

const size_t ARG_RESIZE = 5;
const int MAX_NEW_LEVEL = 65535;
const size_t MIN_NEW_LEVEL = 1;
bool isInteger(const std::string& integer);


int executeOperation(const std::vector<std::string>& arguments,const std::string& method);
int executeInfo(const std::vector<std::string>& arguments, PPMMetadata& metadata);
int executeMaxlevel(const std::vector<std::string>& arguments, PPMMetadata& metadata, const std::string& method);
int executeResize(const std::vector<std::string>& arguments, PPMMetadata& metadata, const std::string& method);
int executeCutfreq(const std::vector<std::string>& arguments, PPMMetadata& metadata, const std::string& method);
int executeCompress(const std::vector<std::string>& arguments, PPMMetadata& metadata, const std::string& method);
int argumentsResize(const std::vector<std::string>& arguments);



#endif //PROGARGS_HPP
