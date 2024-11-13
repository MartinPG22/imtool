//
// Created by mapor on 09/11/2024.
//

#include "utest_soa.hpp"
#include <vector>


/*
// Mock functions and data to simulate the environment
PPMMetadata mockReadPPMMetadata(const std::string& inputPath) {
    return { 100, 100, 255 }; // Mock metadata
}

ImageSOA mockCargarImagenPPMSOA(const std::string& inputPath, PPMMetadata& metadata) {
    return {}; // Mock image data
}

void mockCutfreqSOA(ImageSOA& image, PPMMetadata& metadata, int numberOfColors, const std::string& outputPath) {
    // Mock implementation
}

// Replace the actual functions with mocks
#define readPPMMetadata mockReadPPMMetadata
#define cargarImagenPPMSOA mockCargarImagenPPMSOA
#define cutfreqSOA mockCutfreqSOA

// Test for cutfreq function with valid arguments
TEST(CutfreqTest, ValidArguments) {
    std::vector<std::string> arguments = { "input.ppm", "output.ppm", "cutfreq", "10" };
    PPMMetadata metadata = readPPMMetadata(arguments[0]);
    std::string method = "soa";

    int result = executeCutfreq(arguments, metadata, method);
    EXPECT_EQ(result, 0);
}

// Test for cutfreq function with invalid number of arguments
TEST(CutfreqTest, InvalidNumberOfArguments) {
    std::vector<std::string> arguments = { "input.ppm", "output.ppm", "cutfreq" };
    PPMMetadata metadata = readPPMMetadata(arguments[0]);
    std::string method = "soa";

    int result = executeCutfreq(arguments, metadata, method);
    EXPECT_EQ(result, -1);
}

// Test for cutfreq function with invalid cutfreq argument
TEST(CutfreqTest, InvalidCutfreqArgument) {
    std::vector<std::string> arguments = { "input.ppm", "output.ppm", "cutfreq", "invalid" };
    PPMMetadata metadata = readPPMMetadata(arguments[0]);
    std::string method = "soa";

    int result = executeCutfreq(arguments, metadata, method);
    EXPECT_EQ(result, -1);
}

// Test for cutfreq function with a small image
TEST(CutfreqTest, SmallImage) {
    ImageSOA srcImage;
    srcImage.redChannel = std::vector<uint8_t>{ 255, 0, 255, 0 };
    srcImage.greenChannel = std::vector<uint8_t>{ 0, 255, 255, 0 };
    srcImage.blueChannel = std::vector<uint8_t>{ 0, 0, 255, 255 };

    PPMMetadata metadata = { .width = 2, .height = 2, .max_value = 255 };
    int numberOfColors = 2;
    std::string outputPath = "output.ppm";

    cutfreqSOA(srcImage, metadata, numberOfColors, outputPath);

    // Verify some values of the processed image (mocked, so no actual processing)
    EXPECT_EQ(std::get<std::vector<uint8_t>>(srcImage.redChannel)[0], 255);
    EXPECT_EQ(std::get<std::vector<uint8_t>>(srcImage.greenChannel)[0], 0);
    EXPECT_EQ(std::get<std::vector<uint8_t>>(srcImage.blueChannel)[0], 0);
}
*/
TEST(CombineRGBTest, CombinesCorrectly) {
    EXPECT_EQ(combineRGB(255, 0, 0), 0xFF0000); // Rojo puro
    EXPECT_EQ(combineRGB(0, 255, 0), 0x00FF00); // Verde puro
    EXPECT_EQ(combineRGB(0, 0, 255), 0x0000FF); // Azul puro
    EXPECT_EQ(combineRGB(255, 255, 255), 0xFFFFFF); // Blanco
    EXPECT_EQ(combineRGB(0, 0, 0), 0x000000); // Negro
}

TEST(GetColorDataTest, ConvertsAndSortsCorrectly) {
    std::unordered_map<int, int> const colorFrequency = {
        {combineRGB(255, 0, 0), 5},
        {combineRGB(0, 255, 0), 3},
        {combineRGB(0, 0, 255), 10}
    };

    auto colorData = getColorData(colorFrequency);

    EXPECT_EQ(colorData.size(), 3);
    EXPECT_EQ(std::get<0>(colorData[0]), 3); // El color con menor frecuencia es el verde
    EXPECT_EQ(std::get<1>(colorData[0]), 0); // Valores RGB para el verde
    EXPECT_EQ(std::get<2>(colorData[0]), 255);
    EXPECT_EQ(std::get<3>(colorData[0]), 0);

    EXPECT_EQ(std::get<0>(colorData[2]), 10); // El color con mayor frecuencia es el azul
}

TEST(FindClosestReplacementTest, FindsCorrectColor) {
    std::vector<std::tuple<int, int, int, int>> const colorData = {
        {n_5, n_255, 0, 0},  // Rojo
        {n_3, 0, n_255, 0},  // Verde
        {n_10, 0, 0, n_255}, // Azul
    };

    auto closestToRed = findClosestReplacement(std::make_tuple(n_254, 1, 0), colorData, 0);
    EXPECT_EQ(closestToRed, std::make_tuple(255, 0, 0));

    auto closestToGreen = findClosestReplacement(std::make_tuple(0, n_254, 1), colorData, 0);
    EXPECT_EQ(closestToGreen, std::make_tuple(0, 255, 0));
}

TEST(CreateReplacementMapTest, CreatesCorrectMap) {
    std::vector<std::tuple<int, int, int, int>> const colorData = {
        {n_5, n_255, 0, 0},  // Rojo
        {n_3, 0, n_255, 0},  // Verde
        {n_10, 0, 0, n_255}, // Azul
    };

    int const nColores = 2;
    auto replacementMap = createReplacementMap(colorData, nColores);

    EXPECT_EQ(replacementMap.size(), 2);
    EXPECT_EQ(replacementMap[combineRGB(0, 255, 0)], std::make_tuple(0, 0, 255)); // Rojo reemplazado por Verde
}


TEST(ApplyColorReplacementTest, AppliesReplacementCorrectly) {
    const uint8_t n_255 = 255; // Constante para evitar error en pruebas
    ImageSOA img;
    img.redChannel = std::vector<uint8_t>{n_255, 0, 0};
    img.greenChannel = std::vector<uint8_t>{0, n_255, 0};
    img.blueChannel = std::vector<uint8_t>{0, 0, n_255};

    std::unordered_map<int, std::tuple<int, int, int>> const replacementMap = {
        {combineRGB(n_255, 0, 0), std::make_tuple(0, n_255, 0)}, // Reemplaza rojo por verde
        {combineRGB(0, 0, n_255), std::make_tuple(n_255, 0, 0)}  // Reemplaza azul por rojo
    };

    // Aplicar reemplazo de colores
    applyColorReplacement(img, replacementMap);

    // Comprobar los resultados basados en el tipo de canal de color
    if (std::holds_alternative<std::vector<uint8_t>>(img.redChannel) &&
        std::holds_alternative<std::vector<uint8_t>>(img.greenChannel) &&
        std::holds_alternative<std::vector<uint8_t>>(img.blueChannel)) {

        const auto& red = std::get<std::vector<uint8_t>>(img.redChannel);
        const auto& green = std::get<std::vector<uint8_t>>(img.greenChannel);
        const auto& blue = std::get<std::vector<uint8_t>>(img.blueChannel);

        EXPECT_EQ(red[0], 0);
        EXPECT_EQ(green[0], n_255);
        EXPECT_EQ(blue[0], 0); // Rojo reemplazado por verde

        EXPECT_EQ(red[2], n_255);
        EXPECT_EQ(green[2], 0);
        EXPECT_EQ(blue[2], 0); // Azul reemplazado por rojo
        } else {
            FAIL() << "Canales de color no tienen el tipo esperado (uint8_t)";
        }
}
TEST(CutfreqSOATest, ProcessesImageCorrectly) {
    ImageSOA img;
    img.redChannel = std::vector<uint8_t>{n_255, 0, n_255, 0};
    img.greenChannel = std::vector<uint8_t>{0, n_255, 0, n_255};
    img.blueChannel = std::vector<uint8_t>{0, 0, n_255, n_255};

    PPMMetadata metadata{.width = 2, .height = 2, .max_value = 255};
    metadata.max_value = n_255;

    std::string const outputPath = "output.ppm";
    cutfreqSOA(img, metadata, 2, outputPath);

    // Verificación básica de salida, podría incluir más validación.
    EXPECT_TRUE(std::filesystem::exists(outputPath));
}