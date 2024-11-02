//
// Created by mapor on 02/11/2024.
//

#include "./imgaos/resize.hpp"
#include "./imgaos/resize.cpp"
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

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
    Pixel8 const p1{.r=0, .g=128, .b=255};
    Pixel8 const p2{.r=255, .g=128, .b=0};
    Pixel8 const result = interpolatePixel(p1, p2, 0.5F);
    EXPECT_EQ(result.r, 127);
    EXPECT_EQ(result.g, 128);
    EXPECT_EQ(result.b, 127);
}

// Test pixel interpolation for 16-bit
TEST(PixelInterpolationTest, InterpolatePixel16) {
    Pixel16 const p1{.r=0, .g=32768, .b=65535};
    Pixel16 const p2{.r=65535, .g=32768, .b=0};
    Pixel16 const result = interpolatePixel(p1, p2, 0.5F);
    EXPECT_EQ(result.r, 32767);
    EXPECT_EQ(result.g, 32768);
    EXPECT_EQ(result.b, 32767);
}

// Test image saving for 8-bit pixels
TEST(ImageSavingTest, SavePixelsToPPM8) {
    std::vector<Pixel8> const pixels = {{.r=255, .g=0, .b=0}, {.r=0, .g=255, .b=0}, {.r=0, .g=0, .b=255}};
    std::vector<size_t> const newSize = {1, 3};

    // Guardar en un archivo temporal
    const std::string path = "test_output_8.ppm";
    savePixelsToPPM8(path, pixels, newSize, 255);

    // Verificar contenido del archivo
    std::ifstream file(path, std::ios::binary);
    ASSERT_TRUE(file.is_open());

    std::string header;
    std::getline(file, header); // Leer el encabezado PPM
    EXPECT_EQ(header, "P6");

    size_t width = 0;
    size_t height = 0;
    size_t max_value = 0;
    file >> width >> height >> max_value; // Leer dimensiones y valor máximo
    file.ignore(); // Ignorar el salto de línea después del valor máximo
    EXPECT_EQ(width, 1);
    EXPECT_EQ(height, 3);
    EXPECT_EQ(max_value, 255);

    // Leer los valores de los píxeles
    std::vector<Pixel8> readPixels(height);
    for (size_t i = 0; i < height; ++i) {
        std::array<char, 3> pixelData{}; // Usar std::array en lugar de un arreglo C-style
        file.read(pixelData.data(), pixelData.size()); // Leer los bytes directamente

        // Comprobar si la lectura fue exitosa
        ASSERT_EQ(file.gcount(), static_cast<std::streamsize>(pixelData.size())); // Asegúrate de que se leyeron 3 bytes

        readPixels[i].r = static_cast<uint8_t>(pixelData[0]);
        readPixels[i].g = static_cast<uint8_t>(pixelData[1]);
        readPixels[i].b = static_cast<uint8_t>(pixelData[2]);
    }

    file.close();

    // Limpiar
    if (std::remove(path.c_str()) != 0) { // Verificar si la eliminación fue exitosa
        std::cerr << "Error al eliminar el archivo: " << path << '\n';
    }

    // Comprobar los valores de los píxeles
    EXPECT_EQ(readPixels[0].r, 255);
    EXPECT_EQ(readPixels[1].g, 255);
    EXPECT_EQ(readPixels[2].b, 255);
}

// Add more tests for other functions as necessary...

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
