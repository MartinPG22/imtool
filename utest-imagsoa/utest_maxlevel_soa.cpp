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
    // CHECK: u_32768 is 0.5 in 8-bit y u_16384 is 0.25
    std::vector<uint8_t> const expectedR = {u_255, u_0, u_255, u_0};
    std::vector<uint8_t> const expectedG = {u_128, u_255, u_128, u_255};
    std::vector<uint8_t> const expectedB = {u_64, u_128, u_64, u_128};

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
    std::vector<uint8_t> const expectedG = {u_128, u_255};
    std::vector<uint8_t> const expectedB = {u_64, u_128};

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








// Test Metadata Handling for SOA Format in uint8_t
TEST(MaxLevelSOATest, HandlesMetadataCorrectlySOA) {
    ImageSOA srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 1, .max_value = 65535}; // Original max_value is 65535 for 16-bit
    int const newMaxLevel = 100; // New max level
    std::string const outputPath = "output.ppm";
    // Input image with 16-bit pixels in SOA format (separate R, G, B channels)
    srcImage.redChannel = std::vector<uint8_t>{u_255, u_0}; // Max value and 0 for the first pixel
    srcImage.greenChannel = std::vector<uint8_t>{u_128, u_255}; // Mid and max values for green
    srcImage.blueChannel = std::vector<uint8_t>{u_64, u_128}; // Mid values for blue

    // Act
    const auto imageTrans = maxlevelSOA(srcImage, metadata, newMaxLevel, outputPath);

    // Check if the number of pixels in the output is correct based on metadata
    const auto& outRed = std::get<std::vector<uint8_t>>(imageTrans.redChannel);
    const auto& outGreen = std::get<std::vector<uint8_t>>(imageTrans.greenChannel);
    const auto& outBlue = std::get<std::vector<uint8_t>>(imageTrans.blueChannel);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outRed.size()));
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outGreen.size()));
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outBlue.size()));
}

// Test for Invalid number of pixels
TEST(MaxLevelSOATest, InvalidNumberOfPixelsSOA) {
    ImageSOA srcImage;
    const PPMMetadata metadata = {.width = 3, .height = 2, .max_value = 255}; // Original max_value is 255
    int const newMaxLevel = 100; // New max level
    std::string const outputPath = "output.ppm";
    // Input image with 8-bit pixels in SOA format (separate R, G, B channels)
    srcImage.redChannel = std::vector<uint8_t>{u_255, u_0};
    srcImage.greenChannel = std::vector<uint8_t>{u_128, u_255};
    srcImage.blueChannel = std::vector<uint8_t>{u_64, u_128};

    // Assert: Expect an exception to be thrown due to incorrect number of pixels
    EXPECT_THROW(
        maxlevelSOA(srcImage, metadata, newMaxLevel, outputPath),
        std::runtime_error
    );
}

// Test for Empty Image in SOA format
TEST(MaxLevelSOATest, EmptyImageSOA) {
    ImageSOA srcImage;
    const PPMMetadata metadata = {.width = 0, .height = 0, .max_value = 255}; // Image dimensions are 3x2
    int const newMaxLevel = 100;
    std::string const outputPath = "output.ppm";

    // Empty image with no pixels in any channel
    srcImage.redChannel = std::vector<uint8_t>{};
    srcImage.greenChannel = std::vector<uint8_t>{};
    srcImage.blueChannel = std::vector<uint8_t>{};

    // Assert: Expect an exception to be thrown due to the empty image
    EXPECT_THROW(
        maxlevelSOA(srcImage, metadata, newMaxLevel, outputPath),
        std::runtime_error
    );
}

// Test Metadata Max Level in SOA format 8 bytes
TEST(MaxLevelSOATest, HandlesMetadataMaxLevel8) {
    ImageSOA srcImage8;
    int const newMaxLevel = 100;
    std::string const outputPath = "output.ppm";

    // Valid input image with 16-bit pixels in SOA format (separate R, G, B channels)
    srcImage8.redChannel = std::vector<uint8_t>{u_255, u_128};
    srcImage8.greenChannel = std::vector<uint8_t>{u_128, u_255};
    srcImage8.blueChannel = std::vector<uint8_t>{u_64, u_32};

    // Test case 1: Valid max_value (255)
    const PPMMetadata metadata1 = {.width = 2, .height = 1, .max_value = 255};
    EXPECT_NO_THROW(
        maxlevelSOA(srcImage8, metadata1, newMaxLevel, outputPath)
    );

    // Test case 2: Invalid max_value (0) min
    const PPMMetadata metadata2 = {.width = 2, .height = 1, .max_value = 0};
    EXPECT_THROW(
        maxlevelSOA(srcImage8, metadata2, newMaxLevel, outputPath),
        std::runtime_error
    );

    // Test case 3: Invalid max_value (256) max
    const PPMMetadata metadata3 = {.width = 2, .height = 1, .max_value = 0};
    EXPECT_THROW(
        maxlevelSOA(srcImage8, metadata3, newMaxLevel, outputPath),
        std::runtime_error
    );

    // Test case 4: Negative max_value (e.g., -100)
    const PPMMetadata metadata4 = {.width = 2, .height = 1, .max_value = -100};
    EXPECT_THROW(
        maxlevelSOA(srcImage8, metadata4, newMaxLevel, outputPath),
        std::runtime_error
    );
}

// Test Metadata Max Level in SOA format 16 bytes
TEST(MaxLevelSOATest, HandlesMetadataMaxLevel16) {
    ImageSOA srcImage16;
    int const newMaxLevel = 300;
    std::string const outputPath = "output.ppm";

    // Valid input image with 16-bit pixels in SOA format (separate R, G, B channels)
    srcImage16.redChannel = std::vector<uint16_t>{u_65535, u_32768};
    srcImage16.greenChannel = std::vector<uint16_t>{u_32768, u_65535};
    srcImage16.blueChannel = std::vector<uint16_t>{u_16384, u_8192};

    // Test case 1: Valid max_value (65535)
    const PPMMetadata metadata1 = {.width = 2, .height = 1, .max_value = 65535};
    EXPECT_NO_THROW(
        maxlevelSOA(srcImage16, metadata1, newMaxLevel, outputPath)
    );

    // Test case 2: Invalid max_value (255) min
    const PPMMetadata metadata2 = {.width = 2, .height = 1, .max_value = 255};
    EXPECT_THROW(
        maxlevelSOA(srcImage16, metadata2, newMaxLevel, outputPath),
        std::runtime_error
    );

    // Test case 3: Very high max_value (65536) max
    const PPMMetadata metadata3 = {.width = 2, .height = 1, .max_value = 65536};
    EXPECT_NO_THROW(
        maxlevelSOA(srcImage16, metadata3, newMaxLevel, outputPath)
    );

    // Test case 4: Negative max_value (e.g., -100)
    const PPMMetadata metadata4 = {.width = 2, .height = 1, .max_value = -100};
    EXPECT_THROW(
        maxlevelSOA(srcImage16, metadata4, newMaxLevel, outputPath),
        std::runtime_error
    );
}

// Test image with different width/height
TEST(MaxLevelSOATest, HandlesNonSquareImage) {
    ImageSOA srcImage;
    const PPMMetadata metadata = {.width = 3, .height = 2, .max_value = 255}; // Non-square dimensions (3x2)
    int const newMaxLevel = 100; // New max level
    std::string const outputPath = "output.ppm";

    // Input image with 8-bit pixels in SOA format (separate R, G, B channels)
    srcImage.redChannel = std::vector<uint8_t>{u_255, u_0, u_100, u_200, u_128, u_50};
    srcImage.greenChannel = std::vector<uint8_t>{u_128, u_255, u_50, u_100, u_64, u_25};
    srcImage.blueChannel = std::vector<uint8_t>{u_64, u_128, u_25, u_50, u_32, u_12};

    // Expected output after scaling
    std::vector<uint8_t> const expectedR = {100, 0, 39, 78, 50, 19};
    std::vector<uint8_t> const expectedG = {50, 100, 19, 39, 25, 9};
    std::vector<uint8_t> const expectedB = {25, 50, 9, 19, 12, 4};

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
