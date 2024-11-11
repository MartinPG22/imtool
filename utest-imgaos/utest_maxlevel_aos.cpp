//
// Created by asanch on 11/3/24.
//

#include "./imgaos/maxlevel.hpp"

#include <gtest/gtest.h>

class MaxLevelAOSTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any common data for tests if needed
    }
};

// Test for 8-bit pixel image with valid input
TEST_F(MaxLevelAOSTest, TestMaxLevel8BitSuccess) {
    ImageAOS srcImage;
    srcImage.pixels = std::vector<Pixel8>{{255, 128, 64}, {0, 64, 255}, {100, 200, 50}};

    PPMMetadata metadata{};
    metadata.max_value = 255;

    std::string const outputPath = "output.ppm";

    // Call the function
    int result = maxlevelAOS(srcImage, metadata, 127, outputPath);

    // Check results
    EXPECT_EQ(result, 0);
    // Add more expectations to check the pixel values in the saved image if necessary
}

// Test for 16-bit pixel image with valid input
TEST_F(MaxLevelAOSTest, TestMaxLevel16BitSuccess) {
    ImageAOS srcImage;
    srcImage.pixels = std::vector<Pixel16>{{65535, 32768, 16384}, {0, 32768, 65535}, {32768, 49152, 8192}};

    PPMMetadata metadata;
    metadata.max_value = 65535;

    std::string outputPath = "output.ppm";

    // Call the function
    int result = maxlevelAOS(srcImage, metadata, 255, outputPath);

    // Check results
    EXPECT_EQ(result, 0);
    // Add more expectations to check the pixel values in the saved image if necessary
}

// Test for unsupported pixel format
TEST_F(MaxLevelAOSTest, TestMaxLevelUnsupportedFormat) {
    ImageAOS srcImage;
    // Let's assume we have an unsupported pixel format
    srcImage.pixels = "unsupported format";

    PPMMetadata metadata{};
    metadata.max_value = 255;

    const std::string outputPath = "output.ppm";

    // Expect runtime_error
    EXPECT_THROW(maxlevelAOS(srcImage, metadata, 127, outputPath), std::runtime_error);
}

// Test for file write error (assuming you have a mechanism to mock file writing)
TEST_F(MaxLevelAOSTest, TestMaxLevelFileWriteError) {
    ImageAOS srcImage;
    srcImage.pixels = std::vector<Pixel8>{{255, 128, 64}, {0, 64, 255}, {100, 200, 50}};

    PPMMetadata metadata{};
    metadata.max_value = 255;

    std::string const outputPath = "invalid_path/output.ppm"; // Simulate invalid path

    // Expect runtime_error due to file write error
    EXPECT_THROW(maxlevelAOS(srcImage, metadata, 127, outputPath), std::runtime_error);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}