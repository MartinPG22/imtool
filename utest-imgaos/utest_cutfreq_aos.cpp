//
// Created by mapor on 04/11/2024.
//

#include "utest_aos.hpp"

template <typename T>
bool colorsAreEqual(const Color<T>& color1, const Color<T>& color2) {
    return color1.r() == color2.r() && color1.g() == color2.g() && color1.b() == color2.b();
}
// Prueba para un pixel de 8 bits
TEST(ColorTest, CombineRGB_Pixel8) {
    Color<uint8_t> color8(255, 128, 64); // Color con valores máximos de 8 bits
    uint64_t combined = combineRGB(color8);

    // Esperado: (255 << 16) | (128 << 8) | 64
    uint64_t expected = (255 << 16) | (128 << 8) | 64;

    EXPECT_EQ(combined, expected);

    // Imprimir resultado
    std::cout << "Combined RGB (Pixel 8): " << combined << std::endl;
}

TEST(ColorTests, CombineAndExtractRGB) {
    Color<uint8_t> const color(128, 64, 32);
    uint64_t const combined = combineRGB(color);
    Color<uint8_t> const extracted = extractRGB8<uint8_t>(combined);

    EXPECT_TRUE(colorsAreEqual(color, extracted));
}

TEST(ImageTests, CountColorFrequency) {
    ImageAOS image;
    image.pixels = std::vector<Pixel8>{{.r = 128, .g = 64, .b = 32}, {.r = 128, .g = 64, .b = 32}, {.r = 255, .g = 255, .b = 255}};

    std::unordered_map<uint64_t, int> colorFrequency;
    countColorFrequency(image, colorFrequency);

    Color<uint8_t> const color1(128, 64, 32);
    Color<uint8_t> const color2(255, 255, 255);
    EXPECT_EQ(colorFrequency[combineRGB(color1)], 2);
    EXPECT_EQ(colorFrequency[combineRGB(color2)], 1);
}

TEST(ImageTests2, CountColorFrequency) {
    ImageAOS image2;
    image2.pixels = std::vector<Pixel16>{{.r = 3000, .g = 3000, .b = 3000}, {.r = 3000, .g = 3000, .b = 3000}, {.r = 400, .g = 400, .b = 400}};

    std::unordered_map<uint64_t, int> colorFrequency;
    countColorFrequency(image2, colorFrequency);

    Color<uint16_t> const color1(3000, 3000, 3000);
    Color<uint16_t> const color2(400, 400, 400);
    EXPECT_EQ(colorFrequency[combineRGB48(color1)], 2);
    EXPECT_EQ(colorFrequency[combineRGB48(color2)], 1);
}

#include <gtest/gtest.h>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <iostream>

// Asumiendo que ya tienes definidas las funciones combineRGB y sortColorsByFrequency
// y que Color es una plantilla que acepta uint8_t.

// Ejemplo de pruebas unitarias
TEST(ColorTests, SortColorsByFrequency) {
    std::unordered_map<uint64_t, int> colorFrequency;
    colorFrequency[combineRGB(Color<int>(255, 0, 0))] = 5;
    colorFrequency[combineRGB(Color<int>(0, 255, 0))] = 3;
    colorFrequency[combineRGB(Color<int>(0, 0, 255))] = 5; // Mismo que el rojo, ordenará por RGB

    std::vector<std::tuple<int, Color<int>>> colorData;
    sortColorsByFrequency(colorFrequency, colorData, 255);

    // Imprimir el resultado para depuración
    std::cout << "Resultados de colorData:" << '\n';
    for (const auto& color : colorData) {
        std::cout << "Frecuencia: " << std::get<0>(color) << ", Color: ("
                  << std::get<1>(color).r() << ", "
                  << std::get<1>(color).g() << ", "
                  << std::get<1>(color).b() << ")" << '\n';
    }

    // Esperados: {0, 0, 255} y {255, 0, 0} deben estar en el mismo nivel de frecuencia
    EXPECT_TRUE(colorsAreEqual(std::get<1>(colorData[1]), Color<int>(0, 0, 255))); // Azul con frecuencia 5
    EXPECT_TRUE(colorsAreEqual(std::get<1>(colorData[2]), Color<int>(255, 0, 0))); // Rojo con frecuencia 5
    EXPECT_TRUE(colorsAreEqual(std::get<1>(colorData[0]), Color<int>(0, 255, 0))); // Verde con frecuencia 3
}

// Prueba de separación de colores
TEST(ColorTests, SplitColors) {
    std::vector<std::tuple<int, Color<int>>> const colorData = {
        {10, Color<int>(255, 0, 0)}, // Color a eliminar
        {5, Color<int>(0, 255, 0)},
        {3, Color<int>(0, 0, 255)}
    };

    ColorSplit splitResult = splitColors(colorData, 1);
    EXPECT_EQ(splitResult.colorsToRemove.size(), 1); // 1 color a eliminar
    EXPECT_EQ(splitResult.colorsToKeep.size(), 2);   // 2 colores a mantener
    EXPECT_TRUE(colorsAreEqual(std::get<1>(splitResult.colorsToRemove[0]), Color<int>(255, 0, 0))); // Color a eliminar
}

// Prueba de encontrar el color más cercano
TEST(ColorTests, FindClosestColor) {
    Color<int> const colorToRemove(100, 100, 100);
    std::vector<std::tuple<int, Color<int>>> const colorsToKeep = {
        {5, Color<int>(90, 90, 90)},    // Color más cercano
        {3, Color<int>(200, 200, 200)}
    };

    Color<int> const closestColor = findClosestColor(colorToRemove, colorsToKeep);
    EXPECT_TRUE(colorsAreEqual(closestColor, Color<int>(90, 90, 90))); // Debería ser el más cercano
}

// Verificación de que la función aplica el reemplazo correctamente
TEST(ImageTests, ApplyColorReplacement) {
    ImageAOS image;
    image.pixels = std::vector<Pixel8>{{.r=128, .g=64, .b=32}, {.r=128, .g=64, .b=32}, {.r=255, .g=255, .b=255}};

    std::unordered_map<u_int64_t, Color<int>> replacementMap;
    replacementMap[combineRGB(Color<int>(128, 64, 32))] = Color<int>(0, 0, 0); // Reemplazar color (128, 64, 32) con (0, 0, 0)

    applyColorReplacement(image, replacementMap);

    const auto& pixels8 = std::get<std::vector<Pixel8>>(image.pixels);

    EXPECT_EQ(pixels8[0].r, 0);
    EXPECT_EQ(pixels8[0].g, 0);
    EXPECT_EQ(pixels8[0].b, 0);
    EXPECT_EQ(pixels8[1].r, 0);
    EXPECT_EQ(pixels8[1].g, 0);
    EXPECT_EQ(pixels8[1].b, 0);
    EXPECT_EQ(pixels8[2].r, 255);  // Este color no está en el mapa de reemplazo
    EXPECT_EQ(pixels8[2].g, 255);
    EXPECT_EQ(pixels8[2].b, 255);
}

// Prueba de reemplazo de color en imagen con píxeles de tipo Pixel16
TEST(ImageTests, ApplyColorReplacementPixel16) {
    ImageAOS image;
    image.pixels = std::vector<Pixel16>{{.r=30000, .g=15000, .b=7500}, {.r=30000, .g=15000, .b=7500}, {.r=65535, .g=65535, .b=65535}};

    std::unordered_map<uint64_t, Color<int>> replacementMap;
    Color<int> colorToReplace(30000, 15000, 7500);
    replacementMap[combineRGB48(colorToReplace)] = Color<int>(0, 0, 0); // Usar combineRGB48

    applyColorReplacement(image, replacementMap);

    const auto& pixels16 = std::get<std::vector<Pixel16>>(image.pixels);

    EXPECT_EQ(pixels16[0].r, 0);
    EXPECT_EQ(pixels16[0].g, 0);
    EXPECT_EQ(pixels16[0].b, 0);
    EXPECT_EQ(pixels16[1].r, 0);
    EXPECT_EQ(pixels16[1].g, 0);
    EXPECT_EQ(pixels16[1].b, 0);
    EXPECT_EQ(pixels16[2].r, 65535);  // Este color no está en el mapa de reemplazo
    EXPECT_EQ(pixels16[2].g, 65535);
    EXPECT_EQ(pixels16[2].b, 65535);
}


// Prueba principal para la función cutfreq
TEST(MainFunctionTests, CutFreq) {
    ImageAOS image;
    image.pixels = std::vector<Pixel8>{{.r=128, .g=64, .b=32}, {.r=255, .g=255, .b=255}, {.r=128, .g=64, .b=32}};

    PPMMetadata metadata{};
    metadata.max_value = 255; // Establece el valor máximo de color de la imagen
    int constexpr nColores = 1; // Se eliminará el color menos frecuente
    std::string const outputPath = "output.ppm";

    cutfreq(image, metadata, nColores, outputPath);

    const auto& pixels8 = std::get<std::vector<Pixel8>>(image.pixels);

    EXPECT_EQ(pixels8[0].r, 128); // El color (128, 64, 32) debe permanecer
    EXPECT_EQ(pixels8[0].g, 64);
    EXPECT_EQ(pixels8[0].b, 32);

    EXPECT_EQ(pixels8[1].r, 128); // El color (128, 64, 32) debe permanecer
    EXPECT_EQ(pixels8[1].g, 64);
    EXPECT_EQ(pixels8[1].b, 32);

    EXPECT_EQ(pixels8[2].r, 128); // El color (128, 64, 32) debe permanecer
    EXPECT_EQ(pixels8[2].g, 64);
    EXPECT_EQ(pixels8[2].b, 32);
}

// Prueba principal para la función cutfreq con píxeles de tipo Pixel16
TEST(MainFunctionTests, CutFreqPixel16) {
    ImageAOS image;
    image.pixels = std::vector<Pixel16>{{.r=30000, .g=15000, .b=7500}, {.r=65534, .g=65535, .b=65535}, {.r=30000, .g=15000, .b=7500}};

    PPMMetadata metadata{};
    metadata.max_value = 65535; // Establece el valor máximo de color para Pixel16
    int const nColores = 1; // Se eliminará el color menos frecuente
    std::string const outputPath = "output.ppm";

    cutfreq(image, metadata, nColores, outputPath);

    const auto& pixels16 = std::get<std::vector<Pixel16>>(image.pixels);

    EXPECT_EQ(pixels16[0].r, 30000);
    EXPECT_EQ(pixels16[0].g, 15000);
    EXPECT_EQ(pixels16[0].b, 7500);

    EXPECT_EQ(pixels16[1].r, 30000);
    EXPECT_EQ(pixels16[1].g, 15000);
    EXPECT_EQ(pixels16[1].b, 7500);

    EXPECT_EQ(pixels16[2].r, 30000);
    EXPECT_EQ(pixels16[2].g, 15000);
    EXPECT_EQ(pixels16[2].b, 7500);
}

// Otra prueba principal para la función cutfreq
TEST(MainFunctionTests2, CutFreq) {
    ImageAOS image;
    image.pixels = std::vector<Pixel16>{{.r=128, .g=64, .b=32}, {.r=300, .g=255, .b=255}, {.r=128, .g=64, .b=32}};

    PPMMetadata metadata{};
    metadata.max_value = 300; // Establece el valor máximo de color de la imagen
    int constexpr nColores = 1; // Se eliminará el color menos frecuente
    std::string const outputPath = "output.ppm";

    cutfreq(image, metadata, nColores, outputPath);

    const auto& pixels16 = std::get<std::vector<Pixel16>>(image.pixels);

    EXPECT_EQ(pixels16[0].r, 128); // El color (128, 64, 32) debe permanecer
    EXPECT_EQ(pixels16[0].g, 64);
    EXPECT_EQ(pixels16[0].b, 32);

    EXPECT_EQ(pixels16[1].r, 128); // El color (128, 64, 32) debe permanecer
    EXPECT_EQ(pixels16[1].g, 64);
    EXPECT_EQ(pixels16[1].b, 32);

    EXPECT_EQ(pixels16[2].r, 128); // El color (128, 64, 32) debe permanecer
    EXPECT_EQ(pixels16[2].g, 64);
    EXPECT_EQ(pixels16[2].b, 32);
}

