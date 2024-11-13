//
// Created by asanch on 11/3/24.
//

#include "utest_aos.hpp"


/**
 * @brief Test for maxlevelAOS function.
 *
 * This test verifies that the maxlevelAOS function correctly scales the pixel values of an 8-bit
 * pixel square image in AOS format to a new maximum level also in 8-bit.
 */
TEST(MaxLevelAOSTest, ConvertsPixel8CSAosRegular) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 2, .max_value = 255}; // Original max_value is 255
    int const newMaxLevel = 100; // New max level
    std::string const outputPath = "output.ppm";

    // Input image with 8-bit pixels
    srcImage.pixels = std::vector<Pixel8>{
        {.r=u_255, .g=u_128, .b=u_64},
        {.r=u_0, .g=u_255, .b=u_128},
        {.r=u_0, .g=u_255, .b=u_128},
        {.r=u_0, .g=u_255, .b=u_128} };

    // Expected output pixels
    std::vector<Pixel8> const expectedPixels = {
        {.r=u_100, .g=u_50, .b=u_25}, // Scaled: (r, g, b) * newMaxLevel / metadata.max_value
        {.r=u_0, .g=u_100, .b=u_50},
        {.r=u_0, .g=u_100, .b=u_50},
        {.r=u_0, .g=u_100, .b=u_50}
    };

    // Act
    const auto [pixels] = maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outPixels = std::get<std::vector<Pixel8>>(pixels);
    EXPECT_EQ(outPixels, expectedPixels);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outPixels.size()));
}

/**
 * @brief Test for maxlevelAOS function.
 *
 * This test verifies that the maxlevelAOS function correctly scales the pixel values of a 16-bit
 * pixel square image in AOS format to a new maximum level also in 16-bit.
 */
TEST(MaxLevelAOSTest, ConvertsPixel16CSAosRegular) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 2, .max_value = 65535}; // Original max_value is 255
    int const newMaxLevel = 30000; // New max level
    std::string const outputPath = "output.ppm";

    // Input image with 8-bit pixels
    srcImage.pixels = std::vector<Pixel16>{
        {.r=u_65535, .g=u_0, .b=u_65535},
        {.r=u_32768, .g=u_65535, .b=u_32768},
        {.r=u_16384, .g=u_32768, .b=u_16384},
        {.r=u_16384, .g=u_32768, .b=u_16384}
    };

    // Expected output pixels
    std::vector<Pixel16> const expectedPixels = {
        {.r=u_30000, .g=u_0, .b=u_30000}, // Scaled: (r, g, b) * newMaxLevel / metadata.max_value
        {.r=u_15000, .g=u_30000, .b=u_15000},
        {.r=u_7500, .g=u_15000, .b=u_7500},
        {.r=u_7500, .g=u_15000, .b=u_7500}
    };

    // Act
    const auto [pixels] = maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outPixels = std::get<std::vector<Pixel16>>(pixels);
    EXPECT_EQ(outPixels, expectedPixels);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outPixels.size()));
}

/**
 * @brief Test for maxlevelAOS function.
 *
 * This test verifies that the maxlevelAOS function correctly scales the pixel values of an 8-bit
 * pixel rectangle image in AOS format to a new maximum level also in 8-bit.
 */
TEST(MaxLevelAOSTest, ConvertsPixel8CRAosRegular) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 1, .max_value = 255}; // Original max_value is 255
    int const newMaxLevel = 100; // New max level
    std::string const outputPath = "output.ppm";

    // Input image with 8-bit pixels
    srcImage.pixels = std::vector<Pixel8>{{.r=u_255, .g=u_128, .b=u_64}, {.r=u_0, .g=u_255, .b=u_128}};

    // Expected output pixels
    std::vector<Pixel8> const expectedPixels = {
        {.r=u_100, .g=u_50, .b=u_25}, // Scaled: (r, g, b) * newMaxLevel / metadata.max_value
        {.r=u_0, .g=u_100, .b=u_50}
    };

    // Act
    const auto [pixels] = maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outPixels = std::get<std::vector<Pixel8>>(pixels);
    EXPECT_EQ(outPixels, expectedPixels);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outPixels.size()));
}

/**
 * @brief Test for maxlevelAOS function.
 *
 * This test verifies that the maxlevelAOS function correctly scales the pixel values of a 16-bit
 * pixel rectangle image in AOS format to a new maximum level also in 16-bit.
 */
TEST(MaxLevelAOSTest, ConvertsPixel16CRAosRegular) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 1, .max_value = 65535}; // Original max_value is 255
    int const newMaxLevel = 30000; // New max level
    std::string const outputPath = "output.ppm";

    // Input image with 8-bit pixels
    srcImage.pixels = std::vector<Pixel16>{
            {.r=u_65535, .g=u_0, .b=u_65535},
            {.r=u_32768, .g=u_65535, .b=u_32768}
    };

    // Expected output pixels
    std::vector<Pixel16> const expectedPixels = {
        {.r=u_30000, .g=u_0, .b=u_30000}, // Scaled: (r, g, b) * newMaxLevel / metadata.max_value
        {.r=u_15000, .g=u_30000, .b=u_15000}
    };

    // Act
    const auto [pixels] = maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outPixels = std::get<std::vector<Pixel16>>(pixels);
    EXPECT_EQ(outPixels, expectedPixels);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outPixels.size()));
}

/**
 * @brief Test for maxlevelAOS function with minimum invalid max level.
 *
 * This test verifies that the maxlevelAOS function correctly handles the case when the new max level
 * is less than or equal to 0.
 */
TEST(MaxLevelAOSTest, InvalidMaxLevelAOSMin) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width=2, .height=1, .max_value=255}; // Original max_value is 255
    int const newMaxLevel = 0; // Invalid max level
    std::string const outputPath = "output.ppm";

    // Input image with 8-bit pixels
    srcImage.pixels = std::vector<Pixel8>{{.r=u_255, .g=u_128, .b=u_64}, {.r=u_0, .g=u_255, .b=u_128}};

    // Expect runtime_error
    EXPECT_THROW(maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath), std::runtime_error);
}

/**
 * @brief Test for maxlevelAOS function with maximum invalid max level.
 *
 * This test verifies that the maxlevelAOS function correctly handles the case when the new max level
 * exceeds the maximum allowable value of 65535.
 */
TEST(MaxLevelAOSTest, InvalidMaxLevelAOSMax) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width=2, .height=1, .max_value=255}; // Original max_value is 255
    int const newMaxLevel = 65536; // Invalid max level
    std::string const outputPath = "output.ppm";

    // Input image with 8-bit pixels
    srcImage.pixels = std::vector<Pixel8>{{.r=u_255, .g=u_128, .b=u_64}, {.r=u_0, .g=u_255, .b=u_128}};

    // Expect runtime_error
    EXPECT_THROW(maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath), std::runtime_error);
}

/**
 * @brief Test for maxlevelAOS function with invalid metadata.
 *
 * This test verifies that the maxlevelAOS function correctly handles the case when the metadata
 * of the input image is inconsistent with the actual number of pixels.
 */
TEST(MaxLevelAOSTest, MetadataConsistencyAOS) {
    ImageAOS srcImage;
    PPMMetadata const metadata = {.width=3, .height=1, .max_value=255}; // max_value = 255, width = 3, height = 1
    int const newMaxLevel = 100; // New max level
    std::string const outputPath = "output.ppm";

    // Input image with incorrect number of pixels
    srcImage.pixels = std::vector<Pixel8>{{.r=u_255, .g=u_128, .b=u_64}};

    // Act & Assert
    EXPECT_THROW(maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath), std::runtime_error);
}

/**
 * @brief Test for maxlevelAOS function.
 *
 * This test verifies that the maxlevelAOS function correctly scales the pixel values of an 8-bit
 * pixel square image in SOA format to a new maximum level in 16-bit.
 */
TEST(MaxLevelAOSTest, ConvertsPixel8CSAosIregular) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 2, .max_value = 255}; // Original max_value is 255
    int const newMaxLevel = 65535; // New max level
    std::string const outputPath = "output.ppm";

    // Input image with 8-bit pixels
    srcImage.pixels = std::vector<Pixel8>{
            {.r=u_255, .g=u_128, .b=u_64},
            {.r=u_0, .g=u_255, .b=u_128},
            {.r=u_0, .g=u_255, .b=u_128},
            {.r=u_0, .g=u_255, .b=u_128} };

    // Expected output pixels
    std::vector<Pixel16> const expectedPixels = {
        {.r=u_65535, .g=u_32896, .b=u_16448}, // Scaled: (r, g, b) * newMaxLevel / metadata.max_value
        {.r=u_0, .g=u_65535, .b=u_32896},
        {.r=u_0, .g=u_65535, .b=u_32896},
        {.r=u_0, .g=u_65535, .b=u_32896}
    };

    // Act
    const auto [pixels] = maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outPixels = std::get<std::vector<Pixel16>>(pixels);
    EXPECT_EQ(outPixels, expectedPixels);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outPixels.size()));
}

/**
 * @brief Test for maxlevelAOS function.
 *
 * This test verifies that the maxlevelAOS function correctly scales the pixel values of a 16-bit
 * pixel square image in AOS format to a new maximum level in 8-bit.
 */
TEST(MaxLevelAOSTest, ConvertsPixel16CSAosIregular) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 2, .max_value = 65535}; // Original max_value is 255
    int const newMaxLevel = 255; // New max level
    std::string const outputPath = "output.ppm";

    // Input image with 8-bit pixels
    srcImage.pixels = std::vector<Pixel16>{
            {.r=u_65535, .g=u_32768, .b=u_16384},
        {.r=u_65535, .g=u_32768, .b=u_16384},
        {.r=u_65535, .g=u_32768, .b=u_16384},
        {.r=u_65535, .g=u_32768, .b=u_16384},
    };

    // Expected output pixels
    std::vector<Pixel8> const expectedPixels = {
        {.r=u_255, .g=u_127, .b=u_63}, // Scaled: (r, g, b) * newMaxLevel / metadata.max_value
        {.r=u_255, .g=u_127, .b=u_63},
        {.r=u_255, .g=u_127, .b=u_63},
        {.r=u_255, .g=u_127, .b=u_63}
    };

    // Act
    const auto [pixels] = maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outPixels = std::get<std::vector<Pixel8>>(pixels);
    EXPECT_EQ(outPixels, expectedPixels);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outPixels.size()));
}

/**
 * @brief Test for maxlevelAOS function.
 *
 * This test verifies that the maxlevelAOS function correctly scales the pixel values of an 8-bit
 * pixel rectangle image in SOA format to a new maximum level in 16-bit.
 */
TEST(MaxLevelAOSTest, ConvertsPixel8CRAosIregular) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 1, .max_value = 255}; // Original max_value is 255
    int const newMaxLevel = 65535; // New max level
    std::string const outputPath = "output.ppm";

    // Input image with 8-bit pixels
    srcImage.pixels = std::vector<Pixel8>{
                {.r=u_255, .g=u_128, .b=u_64},
                {.r=u_0, .g=u_255, .b=u_128}
    };

    // Expected output pixels
    std::vector<Pixel16> const expectedPixels = {
        {.r=u_65535, .g=u_32896, .b=u_16448}, // Scaled: (r, g, b) * newMaxLevel / metadata.max_value
        {.r=u_0, .g=u_65535, .b=u_32896}
    };

    // Act
    const auto [pixels] = maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outPixels = std::get<std::vector<Pixel16>>(pixels);
    EXPECT_EQ(outPixels, expectedPixels);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outPixels.size()));
}

/**
 * @brief Test for maxlevelAOS function.
 *
 * This test verifies that the maxlevelAOS function correctly scales the pixel values of a 16-bit
 * pixel rectangle image in AOS format to a new maximum level in 8-bit.
 */
TEST(MaxLevelAOSTest, ConvertsPixel16CRAosIregular) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 1, .max_value = 65535}; // Original max_value is 255
    int const newMaxLevel = 255; // New max level
    std::string const outputPath = "output.ppm";

    // Input image with 8-bit pixels
    srcImage.pixels = std::vector<Pixel16>{
                {.r=u_65535, .g=u_32768, .b=u_16384},
            {.r=u_65535, .g=u_32768, .b=u_16384}
    };

    // Expected output pixels
    std::vector<Pixel8> const expectedPixels = {
        {.r=u_255, .g=u_127, .b=u_63}, // Scaled: (r, g, b) * newMaxLevel / metadata.max_value
        {.r=u_255, .g=u_127, .b=u_63}
    };

    // Act
    const auto [pixels] = maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outPixels = std::get<std::vector<Pixel8>>(pixels);
    EXPECT_EQ(outPixels, expectedPixels);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outPixels.size()));
}

