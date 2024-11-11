//
// Created by mapor on 09/11/2024.
//

#include "utest_soa.hpp"
#include "./imgsoa/resize.hpp"
#include "./imgsoa/resize.cpp"
#include "./imgsoa/imagesoa.hpp"
#include "./imgsoa/imagesoa.cpp"




// Prueba de interpolación de 8 bits
TEST(InterpolationTest, Interpolate8SOA) {
    uint8_t const v00 = 100;
    uint8_t const v01 = 200;
    float const ttt = 0.5F; // Interpolación al 50%

    uint8_t const result = interpolate8SOA(v00, v01, ttt);

    // La interpolación debe devolver un valor entre v00 y v01
    // Interpolación al 50% => (100 * (1 - 0.5)) + (200 * 0.5) = 150
    EXPECT_EQ(result, 150);
}

// Prueba de interpolación de 16 bits
TEST(InterpolationTest, Interpolate16SOA) {
    uint16_t const v00 = 1000;
    uint16_t const v01 = 2000;
    float const ttt = 0.5F; // Interpolación al 50%

    uint16_t const result = interpolate16SOA(v00, v01, ttt);

    // La interpolación debe devolver un valor entre v00 y v01
    // Interpolación al 50% => (1000 * (1 - 0.5)) + (2000 * 0.5) = 1500
    EXPECT_EQ(result, 1500);
}

// Prueba de interpolación de un píxel genérico
TEST(InterpolationTest, InterpolatePixelSOA) {
    uint8_t const v00 = 100;
    uint8_t const v01 = 200;
    float const ttt = 0.25F; // Interpolación al 25%

    uint8_t const result = interpolatePixelSOA(v00, v01, ttt);

    // La interpolación al 25% => (100 * (1 - 0.25)) + (200 * 0.25) = 125
    EXPECT_EQ(result, 125);
}

// Prueba de obtención de un píxel
TEST(ResizeTest, GetPixelSOA) {
    // Imagen de 2x2 (simplificada para pruebas)
    std::vector<uint8_t> const redChannel = {255, 0, 255, 0};
    std::vector<uint8_t> const greenChannel = {0, 255, 255, 0};
    std::vector<uint8_t> const blueChannel = {0, 0, 255, 255};

    size_t const width = 2;
    size_t const hor = 1;
    size_t const ver = 1; // Obtener el píxel en la posición (1, 1)

    uint8_t const pixel = getPixelSOA(redChannel, width, hor, ver);

    // En este caso, el píxel en la posición (1, 1) es el segundo valor en el canal rojo, que es 0
    EXPECT_EQ(pixel, 0);
}

// Prueba de redimensionamiento de los píxeles de la imagen
TEST(ResizeTest, ResizePixelsSOA) {
    // Imagen de 2x2
    std::vector<uint8_t> const srcPixels = {255, 0, 255, 0, 0, 255, 255, 0, 0, 0, 255, 255};
    PPMMetadata const metadata = {.width = 2, .height = 2, .max_value = 255};

    size_t const newWidth = 4;
    size_t const newHeight = 4;

    // Redimensionar
    std::vector<uint8_t> resizedPixels = resizePixelsSOA(srcPixels, metadata, newWidth, newHeight);

    // Verificar que el tamaño de la imagen redimensionada es el esperado (4x4)
    EXPECT_EQ(resizedPixels.size(), 4 * 4);

    // Verificar algunos valores de píxeles redimensionados (los valores dependerán de la interpolación)
    // Suponiendo que la interpolación funciona correctamente, estos valores deben estar en el rango de los colores originales
    EXPECT_EQ(resizedPixels[0], 255);  // Primer píxel, debería ser 255 (rojo)
    EXPECT_EQ(resizedPixels[1], 127);  // Interpolado entre 255 y 0
    EXPECT_EQ(resizedPixels[5], 127);  // Otro valor interpolado
    EXPECT_EQ(resizedPixels[15], 0);   // Último píxel, debería ser 0 (verde)
}

// Test de redimensionamiento completo de la imagen
TEST(ResizeTest, ResizeSOATest) {
    // Crear una imagen de prueba
    ImageSOA srcImage;
    srcImage.redChannel = std::vector<uint8_t>{n_255, 0, n_255, 0};
    srcImage.greenChannel = std::vector<uint8_t>{0, n_255, n_255, 0};
    srcImage.blueChannel = std::vector<uint8_t>{0, 0, n_255, n_255};

    PPMMetadata const metadata = {.width = 2, .height = 2, .max_value = n_255};
    std::vector<size_t> const newSize = {4, 4};

    // Llamar a la función de redimensionamiento
    ImageSOA dstImage = resizeSOA(srcImage, metadata, newSize, "resized_image.ppm");

    // Usamos std::get para acceder al vector y luego usamos size()
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.redChannel).size(), 4 * 4);
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.greenChannel).size(), 4 * 4);
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.blueChannel).size(), 4 * 4);

    // Verificar algunos valores de los píxeles redimensionados
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.redChannel)[0], 255);   // Primer píxel, debería ser rojo
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.greenChannel)[0], 0);   // Primer píxel, verde = 0
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.blueChannel)[0], 0);    // Primer píxel, azul = 0

    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.redChannel)[5], 127);   // Un píxel interpolado
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.greenChannel)[5], 127); // Un píxel interpolado
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.blueChannel)[5], 127);  // Un píxel interpolado

    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.redChannel)[15], 0);    // Último píxel, debería ser verde
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.greenChannel)[15], 0); // Último píxel, 0
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.blueChannel)[15], 255);  // Último píxel, azul = 255
}
// Definir una imagen pequeña de ejemplo para las pruebas
ImageSOA createTestImage() {
    ImageSOA image;

    // Crear una imagen pequeña 2x2 (solo para propósitos de prueba)
    image.redChannel = std::vector<uint8_t>{n_255, 0, n_255, 0};   // R: Rojo, Verde
    image.greenChannel = std::vector<uint8_t>{0, n_255, n_255, 0}; // G: Verde, Azul
    image.blueChannel = std::vector<uint8_t>{0, 0, n_255, n_255};  // B: Azul, Rojo

    return image;
}

TEST(ResizeTest, ResizeSOATest_ResizeDown) {
    // Crear una imagen de prueba 4x4
    ImageSOA srcImage;
    srcImage.redChannel = std::vector<uint8_t>{n_255, 0, n_255, 0, n_255, n_255, 0, 0, n_255, 0, 0, 0, n_255, n_255, 0, 0};
    srcImage.greenChannel = std::vector<uint8_t>{0, n_255, 0, 0, n_255, n_255, n_255, 0, n_255, 0, 0, 0, n_255, 0, n_255, 0};
    srcImage.blueChannel = std::vector<uint8_t>{0, 0, n_255, n_255, 0, n_255, n_255, n_255, 0, n_255, n_255, n_255, 0, n_255, n_255, n_255};

    // Meta información de la imagen original 4x4
    PPMMetadata const metadata = {.width = 4, .height = 4, .max_value = n_255};
    std::vector<size_t> const newSize = {2, 2};  // Reducción a 2x2

    // Llamar a la función de redimensionamiento (reducción)
    ImageSOA dstImage = resizeSOA(srcImage, metadata, newSize, "resized_image.ppm");

    // Verificar que la imagen redimensionada tenga el tamaño correcto
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.redChannel).size(), 2 * 2);
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.greenChannel).size(), 2 * 2);
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.blueChannel).size(), 2 * 2);

    // Verificar algunos valores de los píxeles redimensionados
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.redChannel)[0], 255);  // Píxel 1: Rojo
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.greenChannel)[0], 0); // Píxel 1: Verde
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.blueChannel)[0], 0);  // Píxel 1: Azul

    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.redChannel)[1], 255);  // Píxel 2: Rojo
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.greenChannel)[1], 0); // Píxel 2: Verde
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.blueChannel)[1], 255);  // Píxel 2: Azul

    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.redChannel)[2], 255);  // Píxel 2: Rojo
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.greenChannel)[2], 255); // Píxel 2: Verde
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.blueChannel)[2], 0);  // Píxel 2: Azul


    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.redChannel)[3], 0);  // Píxel 2: Rojo
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.greenChannel)[3], 0); // Píxel 2: Verde
    EXPECT_EQ(std::get<std::vector<uint8_t>>(dstImage.blueChannel)[3], 255);  // Píxel 2: Azul
}
