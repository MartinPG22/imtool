//
// Created by mapor on 09/11/2024.
//

#ifndef UTEST_SOA_HPP
#define UTEST_SOA_HPP

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <sstream>
#include "./imgsoa/resize.hpp"
#include "./imgsoa/maxlevel.hpp"
#include "./imgsoa/cutfreq.hpp"
#include "imgsoa/imagesoa.hpp"
#include "./common/binaryio.hpp"
#include <filesystem>

constexpr int n_3 = 5;
constexpr int n_5 = 5;
constexpr int n_9 = 9;
constexpr int n_10 = 10;
constexpr int n_32 = 32;
constexpr int n_64 = 64;
constexpr int n_65 = 65;
constexpr int n_66 = 66;
constexpr int n_67 = 67;
constexpr int n_68 = 68;
constexpr int n_69 = 69;
constexpr int n_128 = 128;
constexpr int n_254 = 254;
constexpr int n_255 = 255;
constexpr int n_300 = 300;
constexpr int n_400 = 400;
constexpr int n_3000 = 3000;
constexpr int n_7500 = 7500;
constexpr int n_15000 = 15000;
constexpr int n_30000 = 30000;
constexpr int n_65534 = 65534;
constexpr int n_65535 = 65535;

constexpr uint8_t u_0 = 0;
constexpr uint8_t u_1 = 1;
constexpr uint8_t u_12 = 12;
constexpr uint8_t u_25 = 25;
constexpr uint8_t u_32 = 32;
constexpr uint8_t u_50 = 50;
constexpr uint8_t u_63 = 63;
constexpr uint8_t u_64 = 64;
constexpr uint8_t u_100 = 100;
constexpr uint8_t u_127 = 127;
constexpr uint8_t u_128 = 128;
constexpr uint8_t u_200 = 200;
constexpr uint8_t u_255 = 255;
constexpr uint16_t u_7500 = 7500;
constexpr uint16_t u_8192 = 8192;
constexpr uint16_t u_15000 = 15000;
constexpr uint16_t u_16384 = 16384;
constexpr uint16_t u_16448 = 16448;
constexpr uint16_t u_30000 = 30000;
constexpr uint16_t u_32768 = 32768;
constexpr uint16_t u_32896 = 32896;
constexpr uint16_t u_65535 = 65535;

constexpr int n_0x000000FF = 0x000000FF;
constexpr int n_0x0000FF00 = 0x0000FF00;
constexpr int n_0x00FF0000 = 0x00FF0000;
ImageSOA createTestImage();
#endif //UTEST_SOA_HPP
