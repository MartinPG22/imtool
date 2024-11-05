//
// Created by mapor on 04/11/2024.
//

#ifndef UTEST_AOS_HPP
#define UTEST_AOS_HPP
#include "./imgaos/cutfreq.hpp"
#include "./imgaos/cutfreq.cpp"
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <sstream>

#include "imgaos/imageaos.hpp"
#include "./common/binaryio.hpp"
constexpr int n_5 = 5;
constexpr int n_32 = 32;
constexpr int n_64 = 64;
constexpr int n_128 = 128;
constexpr int n_255 = 255;
constexpr int n_7500 = 7500;
constexpr int n_15000 = 15000;
constexpr int n_30000 = 30000;
constexpr int n_65535 = 65535;

template <typename T>
bool colorsAreEqual(const Color<T>& color1, const Color<T>& color2);
#endif //UTEST_AOS_HPP
