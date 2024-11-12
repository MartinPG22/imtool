//
// Created by asanch on 11/12/24.
//

#include "utest_soa.hpp"

/**
 * @brief Test for maxlevelSOA function.
 *
 * This test verifies that the maxlevelSOA function correctly scales the pixel values of an 8-bit
 * pixel square image in SOA format to a new maximum level also in 8-bit.
 */
TEST(MaxLevelSOATest, ConvertsPixel8CSSoaRegular) {
    ImageSOA srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 2, .max_value = 255};
    int const newMaxLevel = 100; // New max level
    std::string const outputPath = "output.ppm";
    // Input image with 8-bit pixels in SOA format (separate R, G, B channels)
    srcImage.redChannel = std::vector<uint8_t>{u_255, u_0, u_255, u_0};
    srcImage.greenChannel = std::vector<uint8_t>{u_128, u_255, u_128, u_255};
    srcImage.blueChannel = std::vector<uint8_t>{u_64, u_128, u_64, u_128};

    // Expected output pixels in SOA format after scaling
    std::vector<uint8_t> const expectedR = {u_100, u_0, u_100, u_0};
    std::vector<uint8_t> const expectedG = {u_50, u_100, u_50, u_100};
    std::vector<uint8_t> const expectedB = {u_25, u_50, u_25, u_50};

    // Act
    const auto [redChannel, greenChannel, blueChannel] = maxlevelSOA(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outRed = std::get<std::vector<uint8_t>>(redChannel);
    const auto& outGreen = std::get<std::vector<uint8_t>>(greenChannel);
    const auto& outBlue = std::get<std::vector<uint8_t>>(blueChannel);
    EXPECT_EQ(outRed, expectedR);
    EXPECT_EQ(outGreen, expectedG);
    EXPECT_EQ(outBlue, expectedB);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outRed.size()));
}

/**
 * @brief Test for maxlevelSOA function.
 *
 * This test verifies that the maxlevelSOA function correctly scales the pixel values of a 16-bit
 * pixel square image in SOA format to a new maximum level also in 16-bit.
 */
TEST(MaxLevelSOATest, ConvertsPixel16CSSoaRegular) {
    ImageSOA srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 2, .max_value = 65535};
    int const newMaxLevel = 30000; // New max level
    std::string const outputPath = "output.ppm";
    // Input image with 16-bit pixels in SOA format (separate R, G, B channels)
    srcImage.redChannel = std::vector<uint16_t>{u_65535, u_0, u_65535, u_0};
    srcImage.greenChannel = std::vector<uint16_t>{u_32768, u_65535, u_32768, u_65535};
    srcImage.blueChannel = std::vector<uint16_t>{u_16384, u_32768, u_16384, u_32768};

    // Expected output pixels in SOA format after scaling (scaled to new max value)
    std::vector<uint16_t> const expectedR = {u_30000, u_0, u_30000, u_0};
    std::vector<uint16_t> const expectedG = {u_15000, u_30000, u_15000, u_30000};
    std::vector<uint16_t> const expectedB = {u_7500, u_15000, u_7500, u_15000};

    // Act
    const auto [redChannel, greenChannel, blueChannel] = maxlevelSOA(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outRed = std::get<std::vector<uint16_t>>(redChannel);
    const auto& outGreen = std::get<std::vector<uint16_t>>(greenChannel);
    const auto& outBlue = std::get<std::vector<uint16_t>>(blueChannel);
    EXPECT_EQ(outRed, expectedR);
    EXPECT_EQ(outGreen, expectedG);
    EXPECT_EQ(outBlue, expectedB);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outRed.size()));
}

/**
 * @brief Test for maxlevelSOA function.
 *
 * This test verifies that the maxlevelSOA function correctly scales the pixel values of an 8-bit
 * pixel rectangle image in SOA format to a new maximum level also in 8-bit.
 */
TEST(MaxLevelSOATest, ConvertsPixel8CRSoaRegular) {
    ImageSOA srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 1, .max_value = 255};
    int const newMaxLevel = 100; // New max level
    std::string const outputPath = "output.ppm";
    // Input image with 8-bit pixels in SOA format (separate R, G, B channels)
    srcImage.redChannel = std::vector<uint8_t>{u_255, u_0};
    srcImage.greenChannel = std::vector<uint8_t>{u_128, u_255};
    srcImage.blueChannel = std::vector<uint8_t>{u_64, u_128};

    // Expected output pixels in SOA format after scaling
    std::vector<uint8_t> const expectedR = {u_100, u_0};
    std::vector<uint8_t> const expectedG = {u_50, u_100};
    std::vector<uint8_t> const expectedB = {u_25, u_50};

    // Act
    const auto imageTrans = maxlevelSOA(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outRed = std::get<std::vector<uint8_t>>(imageTrans.redChannel);
    const auto& outGreen = std::get<std::vector<uint8_t>>(imageTrans.greenChannel);
    const auto& outBlue = std::get<std::vector<uint8_t>>(imageTrans.blueChannel);
    EXPECT_EQ(outRed, expectedR);
    EXPECT_EQ(outGreen, expectedG);
    EXPECT_EQ(outBlue, expectedB);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outRed.size()));
}

/**
 * @brief Test for maxlevelSOA function.
 *
 * This test verifies that the maxlevelSOA function correctly scales the pixel values of a 16-bit
 * pixel rectangle image in SOA format to a new maximum level also in 16-bit.
 */
TEST(MaxLevelSOATest, ConvertsPixel16CRSoaRegular) {
    ImageSOA srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 1, .max_value = 65535};
    int const newMaxLevel = 30000; // New max level
    std::string const outputPath = "output.ppm";
    // Input image with 16-bit pixels in SOA format (separate R, G, B channels)
    srcImage.redChannel = std::vector<uint16_t>{u_65535, u_0};
    srcImage.greenChannel = std::vector<uint16_t>{u_32768, u_65535};
    srcImage.blueChannel = std::vector<uint16_t>{u_16384, u_32768};

    // Expected output pixels in SOA format after scaling (scaled to new max value)
    std::vector<uint16_t> const expectedR = {u_30000, u_0};
    std::vector<uint16_t> const expectedG = {u_15000, u_30000};
    std::vector<uint16_t> const expectedB = {u_7500, u_15000};

    // Act
    const auto [redChannel, greenChannel, blueChannel] = maxlevelSOA(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outRed = std::get<std::vector<uint16_t>>(redChannel);
    const auto& outGreen = std::get<std::vector<uint16_t>>(greenChannel);
    const auto& outBlue = std::get<std::vector<uint16_t>>(blueChannel);
    EXPECT_EQ(outRed, expectedR);
    EXPECT_EQ(outGreen, expectedG);
    EXPECT_EQ(outBlue, expectedB);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outRed.size()));
}

/**
 * @brief Test for maxlevelSOA function with minimum invalid max level.
 *
 * This test verifies that the maxlevelSOA function correctly handles the case when the new max level
 * is less than or equal to 0.
 */
TEST(MaxLevelSOATest, InvalidMaxLevelSOAMin) {
    ImageSOA srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 1, .max_value = 255};
    std::string const outputPath = "output.ppm";
    // Input image with 8-bit pixels in SOA format (separate R, G, B channels)
    srcImage.redChannel = std::vector<uint8_t>{u_255, u_0};
    srcImage.greenChannel = std::vector<uint8_t>{u_128, u_255};
    srcImage.blueChannel = std::vector<uint8_t>{u_64, u_128};

    // Test with invalid max level (negative)
    int const invalidMaxLevelNegative = -1;
    EXPECT_THROW(maxlevelSOA(srcImage, metadata, invalidMaxLevelNegative, outputPath), std::runtime_error);

    // Test with invalid max level (zero)
    int const invalidMaxLevelZero = 0;
    EXPECT_THROW(maxlevelSOA(srcImage, metadata, invalidMaxLevelZero, outputPath), std::runtime_error);
}

/**
 * @brief Test for maxlevelSOA function with maximum invalid max level.
 *
 * This test verifies that the maxlevelSOA function correctly handles the case when the new max level
 * exceeds the maximum allowable value of 65535.
 */
TEST(MaxLevelSOATest, InvalidMaxLevelSOAMax) {
    ImageSOA srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 1, .max_value = 255};
    std::string const outputPath = "output.ppm";
    // Input image with 8-bit pixels in SOA format (separate R, G, B channels)
    srcImage.redChannel = std::vector<uint8_t>{u_255, u_0};
    srcImage.greenChannel = std::vector<uint8_t>{u_128, u_255};
    srcImage.blueChannel = std::vector<uint8_t>{u_64, u_128};

    // Test with invalid max level (greater than 65535)
    int const invalidMaxLevelMax = 65536;
    EXPECT_THROW(maxlevelSOA(srcImage, metadata, invalidMaxLevelMax, outputPath), std::runtime_error);
}

/**
 * @brief Test for maxlevelSOA function.
 *
 * This test verifies that the maxlevelSOA function correctly scales the pixel values of an 8-bit
 * pixel square image in SOA format to a new maximum level in 16-bit.
 */
TEST(MaxLevelSOATest, ConvertsPixel8CSSoaIregular) {
    ImageSOA srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 2, .max_value = 255};
    int const newMaxLevel = 65535; // New max level
    std::string const outputPath = "output.ppm";
    // Input image with 8-bit pixels in SOA format (separate R, G, B channels)
    srcImage.redChannel = std::vector<uint8_t>{u_255, u_0, u_255, u_0};
    srcImage.greenChannel = std::vector<uint8_t>{u_128, u_255, u_128, u_255};
    srcImage.blueChannel = std::vector<uint8_t>{u_64, u_128, u_64, u_128};

    // Expected output pixels in SOA format after scaling
    std::vector<uint16_t> const expectedR = {u_65535, u_0, u_65535, u_0};
    std::vector<uint16_t> const expectedG = {u_32896, u_65535, u_32896, u_65535};
    std::vector<uint16_t> const expectedB = {u_16448, u_32896, u_16448, u_32896};

    // Act
    const auto imageTrans = maxlevelSOA(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outRed = std::get<std::vector<uint16_t>>(imageTrans.redChannel);
    const auto& outGreen = std::get<std::vector<uint16_t>>(imageTrans.greenChannel);
    const auto& outBlue = std::get<std::vector<uint16_t>>(imageTrans.blueChannel);
    EXPECT_EQ(outRed, expectedR);
    EXPECT_EQ(outGreen, expectedG);
    EXPECT_EQ(outBlue, expectedB);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outRed.size()));
}

/**
 * @brief Test for maxlevelSOA function.
 *
 * This test verifies that the maxlevelSOA function correctly scales the pixel values of a 16-bit
 * pixel square image in SOA format to a new maximum level in 8-bit.
 */
TEST(MaxLevelSOATest, ConvertsPixel16CSSoaIregular) {
    ImageSOA srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 2, .max_value = 65535};
    int const newMaxLevel = 255; // New max level
    std::string const outputPath = "output.ppm";
    // Input image with 16-bit pixels in SOA format (separate R, G, B channels)
    srcImage.redChannel = std::vector<uint16_t>{u_65535, u_0, u_65535, u_0};
    srcImage.greenChannel = std::vector<uint16_t>{u_32768, u_65535, u_32768, u_65535};
    srcImage.blueChannel = std::vector<uint16_t>{u_16384, u_32768, u_16384, u_32768};

    // Expected output pixels in SOA format after scaling (scaled to new max value)
    std::vector<uint8_t> const expectedR = {u_255, u_0, u_255, u_0};
    std::vector<uint8_t> const expectedG = {u_127, u_255, u_127, u_255};
    std::vector<uint8_t> const expectedB = {u_63, u_127, u_63, u_127};

    // Act
    const auto [redChannel, greenChannel, blueChannel] = maxlevelSOA(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outRed = std::get<std::vector<uint8_t>>(redChannel);
    const auto& outGreen = std::get<std::vector<uint8_t>>(greenChannel);
    const auto& outBlue = std::get<std::vector<uint8_t>>(blueChannel);
    EXPECT_EQ(outRed, expectedR);
    EXPECT_EQ(outGreen, expectedG);
    EXPECT_EQ(outBlue, expectedB);
}

/**
 * @brief Test for maxlevelSOA function.
 *
 * This test verifies that the maxlevelSOA function correctly scales the pixel values of an 8-bit
 * pixel rectangle image in SOA format to a new maximum level in 16-bit.
 */
TEST(MaxLevelSOATest, ConvertsPixel8CRSoaIregular) {
    ImageSOA srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 1, .max_value = 255};
    int const newMaxLevel = 65535; // New max level
    std::string const outputPath = "output.ppm";
    // Input image with 8-bit pixels in SOA format (separate R, G, B channels)
    srcImage.redChannel = std::vector<uint8_t>{u_255, u_0};
    srcImage.greenChannel = std::vector<uint8_t>{u_128, u_255};
    srcImage.blueChannel = std::vector<uint8_t>{u_64, u_128};

    // Expected output pixels in SOA format after scaling
    std::vector<uint16_t> const expectedR = {u_65535, u_0};
    std::vector<uint16_t> const expectedG = {u_32896, u_65535};
    std::vector<uint16_t> const expectedB = {u_16448, u_32896};

    // Act
    const auto imageTrans = maxlevelSOA(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outRed = std::get<std::vector<uint16_t>>(imageTrans.redChannel);
    const auto& outGreen = std::get<std::vector<uint16_t>>(imageTrans.greenChannel);
    const auto& outBlue = std::get<std::vector<uint16_t>>(imageTrans.blueChannel);
    EXPECT_EQ(outRed, expectedR);
    EXPECT_EQ(outGreen, expectedG);
    EXPECT_EQ(outBlue, expectedB);
}

/**
 * @brief Test for maxlevelSOA function.
 *
 * This test verifies that the maxlevelSOA function correctly scales the pixel values of a 16-bit
 * pixel rectangle image in SOA format to a new maximum level in 8-bit.
 */
TEST(MaxLevelSOATest, ConvertsPixel16CRSoaIregular) {
    ImageSOA srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 1, .max_value = 65535};
    int const newMaxLevel = 255; // New max level
    std::string const outputPath = "output.ppm";
    // Input image with 16-bit pixels in SOA format (separate R, G, B channels)
    srcImage.redChannel = std::vector<uint16_t>{u_65535, u_0};
    srcImage.greenChannel = std::vector<uint16_t>{u_32768, u_65535};
    srcImage.blueChannel = std::vector<uint16_t>{u_16384, u_32768};

    // Expected output pixels in SOA format after scaling (scaled to new max value)
    // CHECK: u_32768 is 0.5 in 8-bit y u_16384 is 0.25
    std::vector<uint8_t> const expectedR = {u_255, u_0};
    std::vector<uint8_t> const expectedG = {u_127, u_255};
    std::vector<uint8_t> const expectedB = {u_63, u_127};

    // Act
    const auto [redChannel, greenChannel, blueChannel] = maxlevelSOA(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outRed = std::get<std::vector<uint8_t>>(redChannel);
    const auto& outGreen = std::get<std::vector<uint8_t>>(greenChannel);
    const auto& outBlue = std::get<std::vector<uint8_t>>(blueChannel);
    EXPECT_EQ(outRed, expectedR);
    EXPECT_EQ(outGreen, expectedG);
    EXPECT_EQ(outBlue, expectedB);
}
