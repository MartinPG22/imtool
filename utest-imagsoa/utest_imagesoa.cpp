//
// Created by asanch on 11/12/24.
//

#include "utest_soa.hpp"

/**
 * @brief Test for cargarImagenPPMtoSOA function.
 *
 * This test verifies that the cargarImagenPPMtoSOA function correctly handles the case when the file
 * is not found.
 */
TEST(CargarImagenPPMtoSOATest, FileNotFound) {
    PPMMetadata metadata{};
    std::string const invalidPath = "no_existe.ppm";
    EXPECT_THROW(cargarImagenPPMtoSOA(invalidPath, metadata), std::runtime_error);
}

/**
 * @brief Test for cargarImagenPPMtoSOA function.
 *
 * This test verifies that the cargarImagenPPMtoSOA function correctly handles the case when the file
 * is not a PPM file.
 */
TEST(CargarImagenPPMtoSOATest, FileNotSupported) {
    PPMMetadata metadata{};
    EXPECT_THROW(cargarImagenPPMtoSOA("archivo.jpg", metadata), std::runtime_error);
}

/**
 * @brief Test for cargarImagenPPMtoSOA function.
 *
 * This test verifies that the cargarImagenPPMtoSOA function correctly handles the case when the file
 * is not in PPM format or is corrupted.
 */
TEST(CargarImagenPPMtoSOATest, FormatNotSupported) {
    std::ofstream archivo("archivo.ppm", std::ios::binary);
    archivo << "P7\n";
    archivo.close();
    PPMMetadata metadata{};
    EXPECT_THROW(cargarImagenPPMtoSOA("archivo.ppm", metadata), std::runtime_error);
}

/**
 * @brief Test for cargarImagenPPMtoSOA function.
 *
 * This test verifies that the cargarImagenPPMtoSOA function correctly handles the case when the file
 * max_value in the metadata does not match.
 */
TEST(CargarImagenPPMtoSOATest, MaxValueInconsistent) {
    std::ofstream archivo("archivo.ppm", std::ios::binary);
    archivo << "P6\n2 2 5\n";
    archivo << "\xFF\x10\x10\x10\xFF\x10\x10\x10\x10\xFF\x10\x10\x10\x10\xFF"; // Pixeles RGB
    archivo.close();

    PPMMetadata metadata{};
    EXPECT_THROW(cargarImagenPPMtoSOA("archivo.ppm", metadata);, std::runtime_error);
}

/**
 * @brief Test for cargarImagenPPMtoSOA function.
 *
 * This test verifies that the cargarImagenPPMtoSOA function correctly handles the case when the file
 * dimensions in the metadata does not match.
 */
TEST(CargarImagenPPMtoSOATest, DimInconsistentMin) {
    std::ofstream archivo("archivo.ppm", std::ios::binary);
    archivo << "P6\n1 1 255\n";
    archivo << "\xFF\x10\x10\x10\xFF\x10\x10\x10\x10\xFF\x10\x10\x10\x10\xFF"; // Pixeles RGB
    archivo.close();

    PPMMetadata metadata{};
    EXPECT_THROW(cargarImagenPPMtoSOA("archivo.ppm", metadata);, std::runtime_error);
}

/**
 * @brief Test for cargarImagenPPMtoSOA function.
 *
 * This test verifies that the cargarImagenPPMtoSOA function correctly handles the case when the file
 * dimensions in the metadata does not match.
 */
TEST(CargarImagenPPMtoSOATest, DimInconsistentMax) {
    std::ofstream archivo("archivo.ppm", std::ios::binary);
    archivo << "P6\n2 3 255\n";
    archivo << "\xFF\x10\x10\x10\xFF\x10\x10\x10\x10\xFF\x10\x10\x10\x10\xFF"; // Pixeles RGB
    archivo.close();

    PPMMetadata metadata{};
    EXPECT_THROW(cargarImagenPPMtoSOA("archivo.ppm", metadata);, std::runtime_error);
}

/**
 * @brief Test for cargarImagenPPMtoSOA function.
 *
 * This test verifies that the cargarImagenPPMtoSOA function correctly handles the case when the file
 * max_value in the metadata does not match.
 */
TEST(CargarImagenPPMtoSOATest, MaxValueIncorrect) {
    std::ofstream archivo("archivo.ppm", std::ios::binary);
    archivo << "P6\n2 2 -5\n";
    archivo << "\xFF\x10\x10\x10\xFF\x10\x10\x10\x10\xFF\x10\x10\x10\x10\xFF"; // Pixeles RGB
    archivo.close();

    PPMMetadata metadata{};
    EXPECT_THROW(cargarImagenPPMtoSOA("archivo.ppm", metadata);, std::runtime_error);
}

/**
 * @brief Test for cargarImagenPPMtoSOA function.
 *
 * This test verifies that the cargarImagenPPMtoSOA function correctly handles the case when the file
 * dimensions in the metadata does not match.
 */
TEST(CargarImagenPPMtoSOATest, DimIncorrectMin) {
    std::ofstream archivo("archivo.ppm", std::ios::binary);
    archivo << "P6\n2 -2 255\n";
    archivo << "\xFF\x10\x10\x10\xFF\x10\x10\x10\x10\xFF\x10\x10\x10\x10\xFF"; // Pixeles RGB
    archivo.close();

    PPMMetadata metadata{};
    EXPECT_THROW(cargarImagenPPMtoSOA("archivo.ppm", metadata);, std::runtime_error);
}

/**
 * @brief Test for cargarImagenPPMtoSOA function.
 *
 * This test verifies that the cargarImagenPPMtoSOA function correctly verifies the values of the red,
 * green, and blue channels in the SOA image.
 */
TEST(CargarImagenPPMtoSOATest, RGBChannelsCorrect8) {
    std::ofstream archivo("archivo.ppm", std::ios::binary);
    archivo << "P6\n6 1 255\n";
    archivo << "\xFF\x01\x01\x01\xFF\x01\x01\x01\x01\xFF\x01\x01\x01\x01\xFF\xFF\xFF\xFF";
    archivo.close();

    PPMMetadata metadata{};
    ImageSOA const imagen = cargarImagenPPMtoSOA("archivo.ppm", metadata);

    // Verificar que los valores en los canales sean correctos
    EXPECT_EQ(std::get<std::vector<uint8_t>>(imagen.redChannel)[0], 255);
    EXPECT_EQ(std::get<std::vector<uint8_t>>(imagen.redChannel)[1], 1);
    EXPECT_EQ(std::get<std::vector<uint8_t>>(imagen.redChannel)[2], 1);

    // Verificar que todos los valores en los canales estén en el rango (0, 256)
    for (const auto& channel : {imagen.redChannel, imagen.greenChannel, imagen.blueChannel}) {
        for (const auto& value : std::get<std::vector<uint8_t>>(channel)) {
            EXPECT_GT(value, 0);
            EXPECT_LE(value, 255);
        }
    }
}

/**
 * @brief Test for cargarImagenPPMtoSOA function.
 *
 * This test verifies that the cargarImagenPPMtoSOA function correctly verifies the values of the red,
 * green, and blue channels in the SOA image.
 */
TEST(CargarImagenPPMtoSOATest, RGBChannelsCorrect16) {
    std::ofstream archivo("archivo.ppm", std::ios::binary);
    archivo << "P6\n2 4 65535\n";
    std::vector<uint16_t> const pixeles = {
        0xFFFF, 0xFFFF, 0x0001, 0x0001, 0x0001, 0x0001,
        0xFFFF, 0xFFFF, 0x0001, 0x0001, 0x0001, 0x0001,
        0x0001, 0x0001, 0xFFFF, 0xFFFF, 0x0001, 0x0001,
        0x0001, 0x0001, 0x0001, 0x0001, 0xFFFF, 0xFFFF
    };
    // Escribir cada componente de color (R, G, B) en formato little-endian
    for (uint16_t const pixel : pixeles) {
        archivo.put(static_cast<char>(pixel & MAX_COLOR_VALUE));
        archivo.put(static_cast<char>((pixel >> BYTE_SIZE) & MAX_COLOR_VALUE));
    }
    archivo.close();

    PPMMetadata metadata{};
    ImageSOA const imagen = cargarImagenPPMtoSOA("archivo.ppm", metadata);

    // Verificar que los valores en los canales sean correctos
    EXPECT_EQ(std::get<std::vector<uint16_t>>(imagen.redChannel)[0], 65535);
    EXPECT_EQ(std::get<std::vector<uint16_t>>(imagen.redChannel)[1], 1);
    EXPECT_EQ(std::get<std::vector<uint16_t>>(imagen.redChannel)[2], 65535);

    // Verificar que todos los valores en los canales estén en el rango (0, 65535)
    for (const auto& channel : {imagen.redChannel, imagen.greenChannel, imagen.blueChannel}) {
        for (const auto& value : std::get<std::vector<uint16_t>>(channel)) {
            EXPECT_GT(value, 0);
            EXPECT_LE(value, 65535);
        }
    }
}

/**
 * @brief Test for cargarImagenPPMtoSOA function.
 *
 * This test verifies that the cargarImagenPPMtoSOA function correctly handles the case when the file
 * pixels are 8-bit but the max_value is 16-bit.
 */
TEST(CargarImagenPPMtoSOATest, RGBChannelsIncorrect8) {
    std::ofstream archivo("archivo.ppm", std::ios::binary);
    archivo << "P6\n3 2 5000\n";
    archivo << "\xFF\x01\x01\x01\xFF\x01\x01\x01\x01\xFF\x01\x01\x01\x01\xFF\xFF\xFF\xFF";
    archivo.close();

    PPMMetadata metadata{};
    EXPECT_THROW(cargarImagenPPMtoSOA("archivo.ppm", metadata), std::runtime_error);
}

/**
 * @brief Test for cargarImagenPPMtoSOA function.
 *
 * This test verifies that the cargarImagenPPMtoSOA function correctly handles the case when the file
 * pixels are 8-bit but the max_value is 16-bit.
 */
TEST(CargarImagenPPMtoSOATest, RGBChannelsIncorrect16) {
    std::ofstream archivo("archivo.ppm", std::ios::binary);
    archivo << "P6\n2 5 200\n";
    std::vector<uint16_t> const pixeles = {
        0xFFFF, 0xFFFF, 0x0001, 0x0001, 0x0001, 0x0001,
        0xFFFF, 0xFFFF, 0x0001, 0x0001, 0x0001, 0x0001,
        0x0001, 0x0001, 0xFFFF, 0xFFFF, 0x0001, 0x0001,
        0x0001, 0x0001, 0x0001, 0x0001, 0xFFFF, 0xFFFF
    };
    // Escribir cada componente de color (R, G, B) en formato little-endian
    for (uint16_t const pixel : pixeles) {
        archivo.put(static_cast<char>(pixel & MAX_COLOR_VALUE));
        archivo.put(static_cast<char>((pixel >> BYTE_SIZE) & MAX_COLOR_VALUE));
    }
    archivo.close();

    PPMMetadata metadata{};
    EXPECT_THROW(cargarImagenPPMtoSOA("archivo.ppm", metadata), std::runtime_error);
}


