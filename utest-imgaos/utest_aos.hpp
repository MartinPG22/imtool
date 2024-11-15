//
// Created by mapor on 04/11/2024.
//

#ifndef UTEST_AOS_HPP
#define UTEST_AOS_HPP

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <sstream>
#include "./imgaos/resize.hpp"
#include "./imgaos/maxlevel.hpp"
#include "./imgaos/cutfreq.hpp"
#include "imgaos/imageaos.hpp"
#include "./common/binaryio.hpp"

constexpr int n_5 = 5;
constexpr int n_32 = 32;
constexpr int n_50 = 50;
constexpr int n_64 = 64;
constexpr int n_100 = 100;
constexpr int n_128 = 128;
constexpr int n_150 = 150;
constexpr int n_200 = 200;
constexpr int n_255 = 255;
constexpr int n_300 = 300;
constexpr int n_400 = 400;
constexpr int n_3000 = 3000;
constexpr int n_7500 = 7500;
constexpr int n_15000 = 15000;
constexpr int n_30000 = 30000;
constexpr int n_65534 = 65534;
constexpr int n_65535 = 65535;
constexpr int n_0xFF = 0xFF;

constexpr uint8_t u_0 = 0;
constexpr uint8_t u_25 = 25;
constexpr uint8_t u_50 = 50;
constexpr uint8_t u_63 = 63;
constexpr uint8_t u_64 = 64;
constexpr uint8_t u_100 = 100;
constexpr uint8_t u_127 = 127;
constexpr uint8_t u_128 = 128;
constexpr uint8_t u_255 = 255;
constexpr uint16_t u_16384 = 16384;
constexpr uint16_t u_32768 = 32768;
constexpr uint16_t u_65535 = 65535;
constexpr uint16_t u_7500 = 7500;
constexpr uint16_t u_30000 = 30000;
constexpr uint16_t u_15000 = 15000;
constexpr uint16_t u_16448 = 16448;
constexpr uint16_t u_32896 = 32896;

// Valores constantes para pruebas
constexpr size_t kWidth = 2;
constexpr size_t kHeight = 2;
constexpr uint8_t kMaxValue8 = 255;
constexpr uint16_t kMaxValue16 = 65535;


template <typename T>
bool colorsAreEqual(const Color<T>& color1, const Color<T>& color2);
void crearPPM8Bit(const std::string& filename);
void crearPPM16Bit(const std::string& filename);
std::string getFileName8();
std::string getFileName16();
std::string getOutputPath();
void SetUpTestFiles();
void TearDownTestFiles();
bool leerEncabezadoPPM(std::ifstream& archivo, PPMMetadata& metadata);
void validarPixeles16(const std::vector<Pixel16>& pixels16);
#endif //UTEST_AOS_HPP
