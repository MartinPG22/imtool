//
// Created by mapor on 02/11/2024.
//
#include "utest_aos.hpp"

// Test interpolation functions for 8-bit values
TEST(InterpolationTest, Interpolate8) {
    EXPECT_EQ(interpolate8(0, 255, 0.0F), 0);
    EXPECT_EQ(interpolate8(0, 255, 1.0F), 255);
    EXPECT_EQ(interpolate8(0, 255, 0.5F), 127);
}

// Test interpolation functions for 16-bit values
TEST(InterpolationTest, Interpolate16) {
    EXPECT_EQ(interpolate16(0, 65535, 0.0F), 0);
    EXPECT_EQ(interpolate16(0, 65535, 1.0F), 65535);
    EXPECT_EQ(interpolate16(0, 65535, 0.5F), 32767);
}

// Test pixel interpolation for 8-bit
TEST(PixelInterpolationTest, InterpolatePixel8) {
    Pixel8 constexpr pixel1{.r=0, .g=128, .b=255};
    Pixel8 const pixel2{.r=255, .g=128, .b=0};
    Pixel8 const result = interpolatePixel(pixel1, pixel2, 0.5F);
    EXPECT_EQ(result.r, 127);
    EXPECT_EQ(result.g, 128);
    EXPECT_EQ(result.b, 127);
}

// Test pixel interpolation for 16-bit
TEST(PixelInterpolationTest, InterpolatePixel16) {
    Pixel16 const pixel1{.r=0, .g=32768, .b=65535};
    Pixel16 const pixel2{.r=65535, .g=32768, .b=0};
    Pixel16 const result = interpolatePixel(pixel1, pixel2, 0.5F);
    EXPECT_EQ(result.r, 32767);
    EXPECT_EQ(result.g, 32768);
    EXPECT_EQ(result.b, 32767);
}


