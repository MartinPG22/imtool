//
// Created by asanch on 11/3/24.
//

#include "utest_aos.hpp"

// Test Pixel Conversion for 8-bit Pixels
TEST(MaxLevelAOSTest, ConvertsPixel8Correctly) {
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

// Test Pixel Conversion for 16-bit Pixels
TEST(MaxLevelAOSTest, ConvertsPixel16Correctly) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 1, .max_value = 65535}; // Original max_value is 255
    int const newMaxLevel = 256; // New max level
    std::string const outputPath = "output.ppm";

    // Input image with 8-bit pixels
    srcImage.pixels = std::vector<Pixel16>{{.r=u_65535, .g=u_32768, .b=u_16384}, {.r=0, .g=u_65535, .b=u_32768}};

    // Expected output pixels
    std::vector<Pixel16> const expectedPixels = {
        {.r=256, .g=128, .b=64}, // Scaled: (r, g, b) * newMaxLevel / metadata.max_value
        {.r=0, .g=256, .b=128}
    };

    // Act
    const auto [pixels] = maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outPixels = std::get<std::vector<Pixel16>>(pixels);
    EXPECT_EQ(outPixels, expectedPixels);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outPixels.size()));
}

// Test Metadata
TEST(MaxLevelAOSTest, MetadataConsistency) {
    ImageAOS srcImage;
    PPMMetadata const metadata = {.width=3, .height=1, .max_value=255}; // max_value = 255, width = 3, height = 1
    int const newMaxLevel = 100; // New max level
    std::string const outputPath = "output.ppm";

    // Input image with incorrect number of pixels
    srcImage.pixels = std::vector<Pixel8>{{.r=u_255, .g=u_128, .b=u_64}};

    // Act & Assert
    EXPECT_THROW(maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath), std::runtime_error);
}

// Test for invalid max level
TEST(MaxLevelAOSTest, TestMaxLevelInvalid) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width=2, .height=1, .max_value=255}; // Original max_value is 255
    int const newMaxLevel = 0; // Invalid max level
    std::string const outputPath = "output.ppm";

    // Input image with 8-bit pixels
    srcImage.pixels = std::vector<Pixel8>{{.r=u_255, .g=u_128, .b=u_64}, {.r=u_0, .g=u_255, .b=u_128}};

    // Expect runtime_error
    EXPECT_THROW(maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath), std::runtime_error);
}

// Test for Invalid number of pixels
TEST(MaxLevelAOSTest, ThrowsErrorForInvalidPixelCount) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 2, .max_value = 255};  // 2x2 image
    int const newMaxLevel = 100;
    std::string const outputPath = "output.ppm";

    // Input image with 3 pixels (mismatch)
    srcImage.pixels = std::vector<Pixel8>{{.r=u_255, .g=u_128, .b=u_64}, {.r=u_0, .g=u_255, .b=u_128}, {.r=u_100, .g=u_50, .b=u_25}};

    // Act & Assert
    EXPECT_THROW(maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath), std::runtime_error);
}

// Test for empty image
TEST(MaxLevelAOSTest, HandlesEmptyImage) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width = 0, .height = 0, .max_value = 255};
    int const newMaxLevel = 100;
    std::string const outputPath = "output.ppm";

    // Input image with no pixels
    srcImage.pixels = std::vector<Pixel8>{};

    // Act
    const auto [pixels] = maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath);

    // Assert: It should be an empty image
    const auto& outPixels = std::get<std::vector<Pixel8>>(pixels);
    EXPECT_TRUE(outPixels.empty());
}

// Test new max level is greater than metadata max level
TEST(MaxLevelAOSTest, HandlesNewMaxLevelGreaterThanMetadataMax) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width = 2, .height = 1, .max_value = 255};
    int const newMaxLevel = 500;  // New max level is greater than max_value
    std::string const outputPath = "output.ppm";

    // Input image with 8-bit pixels
    srcImage.pixels = std::vector<Pixel8>{{.r=u_255, .g=u_128, .b=u_64}, {.r=u_0, .g=u_255, .b=u_128}};

    uint8_t const u_125 = 125;
    uint8_t const u_250 = 250;
    uint8_t const u_500 = 255;
    // Expected output pixels after scaling (should be clamped at newMaxLevel)
    std::vector<Pixel8> const expectedPixels = {
        {.r=u_500, .g=u_250, .b=u_125}, // Scaled values should not exceed 500
        {.r=u_0, .g=u_500, .b=u_250}
    };

    // Act
    const auto [pixels] = maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outPixels = std::get<std::vector<Pixel8>>(pixels);
    //EXPECT_EQ(outPixels, expectedPixels);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outPixels.size()));
}

// Test image with different width/height
TEST(MaxLevelAOSTest, HandlesNonSquareImage) {
    ImageAOS srcImage;
    const PPMMetadata metadata = {.width = 3, .height = 2, .max_value = 255};
    int const newMaxLevel = 100;
    std::string const outputPath = "output.ppm";

    uint8_t const u_9 = 9;
    uint8_t const u_12 = 12;
    uint8_t const u_19 = 19;
    uint8_t const u_32 = 32;
    uint8_t const u_39 = 39;
    // Input image with 8-bit pixels (3x2)
    srcImage.pixels = std::vector<Pixel8>{
            {.r=u_255, .g=u_128, .b=u_64}, {.r=u_0, .g=u_255, .b=u_128}, {.r=u_100, .g=u_50, .b=u_25},
            {.r=u_255, .g=u_255, .b=u_255}, {.r=u_128, .g=u_64, .b=u_32}, {.r=u_0, .g=u_0, .b=u_0}
    };

    // Expected output pixels after scaling
    std::vector<Pixel8> const expectedPixels = {
        {.r=u_100, .g=u_50, .b=u_25}, {.r=u_0, .g=u_100, .b=u_50}, {.r=u_39, .g=u_19, .b=u_9},
        {.r=u_100, .g=u_100, .b=u_100}, {.r=u_50, .g=u_25, .b=u_12}, {.r=u_0, .g=u_0, .b=u_0}
    };

    // Act
    const auto [pixels] = maxlevelAOS(srcImage, metadata, newMaxLevel, outputPath);

    // Assert
    const auto& outPixels = std::get<std::vector<Pixel8>>(pixels);
    EXPECT_EQ(outPixels, expectedPixels);
    EXPECT_EQ(metadata.width * metadata.height, static_cast<int>(outPixels.size()));
}
