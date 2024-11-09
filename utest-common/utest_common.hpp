//
// Created by mapor on 09/11/2024.
//

#ifndef UTEST_COMMON_HPP
#define UTEST_COMMON_HPP


#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include "./common/binaryio.hpp"
#include "./common/progargs.hpp"
#include "./imgsoa/resize.hpp"
#include "./imgsoa/cutfreq.hpp"
#include "./imgsoa/maxlevel.hpp"
#include "./imgsoa/compress.hpp"
#include "./imgaos/resize.hpp"
#include "./imgaos/cutfreq.hpp"
#include "./imgaos/maxlevel.hpp"
#include "./imgaos/compress.hpp"



constexpr int n_8 = 8;
constexpr int n_13 = 13;
constexpr int n_255 = 255;
constexpr int n_600 = 600;
constexpr int n_800 = 800;

#endif //UTEST_COMMON_HPP
