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

    PPMMetadata metadata{.width = 6, .height = 1, .max_value = 255};
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

    PPMMetadata metadata{.width = 2, .height = 4, .max_value = 65535};
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

/**
 * @brief Test for saveSOAtoPPMTest function.
 *
 * This test verifies that the saveSOAtoPPMTest function correctly handles the case when the file
 * is not found.
 */
TEST(saveSOAtoPPMTest, FileNotFound) {
    ImageSOA const testImage{
        .redChannel = std::vector<uint8_t>{u_255, u_0, u_0},
        .greenChannel = std::vector<uint8_t>{u_0, u_255, u_0},
        .blueChannel = std::vector<uint8_t>{u_0, u_0, u_255}
    };
    PPMMetadata const testMetadata{
        .width = 1,
        .height = 3,
        .max_value = 255,
    };
    int const maxLevel = 255;
    std::string const invalidPath = "no_existe.pp";
    EXPECT_THROW(saveSOAtoPPM(testImage, testMetadata, maxLevel, invalidPath), std::runtime_error);
}

/**
 * @brief Test for saveSOAtoPPMTest function.
 *
 * This test verifies that the saveSOAtoPPMTest function correctly handles the case when the SOA
 * image channels have different sizes.
 */
TEST(SaveSOAToPPMTests, MismatchedChannelSizes) {
    // Entradas: Canales de diferentes tamaños
    ImageSOA const testImage{
        .redChannel = std::vector<uint8_t>{u_255, u_0, u_0},
        .greenChannel = std::vector<uint8_t>{u_0, u_255},
        .blueChannel = std::vector<uint8_t>{u_0, u_0, u_255}
    };
    PPMMetadata const testMetadata{
        .width = 1,
        .height = 3,
        .max_value = 255,
    };
    int const maxLevel = 255;
    std::string const outputPath = "test_output.ppm";
    // Ejecutar la función
    EXPECT_THROW(saveSOAtoPPM(testImage, testMetadata, maxLevel, outputPath), std::runtime_error);
}

/**
 * @brief Test for saveSOAtoPPMTest function.
 *
 * This test verifies that the saveSOAtoPPMTest function correctly saves the SOA image to a PPM file.
 */
TEST(SaveSOAToPPMTests, ValidImageUint8) {
    ImageSOA const testImage{
        .redChannel = std::vector<uint8_t>{u_255, u_1, u_1},
        .greenChannel = std::vector<uint8_t>{u_1, u_255, u_1},
        .blueChannel = std::vector<uint8_t>{u_1, u_1, u_255}
    };
    PPMMetadata const testMetadata{
        .width = 1,
        .height = 3,
        .max_value = 255,
    };
    int const maxLevel = 255;
    std::string const outputPath = "test_output.ppm";

    // Ejecutar la función
    int const result = saveSOAtoPPM(testImage, testMetadata, maxLevel, outputPath);

    // Validar retorno
    EXPECT_EQ(result, 0); // La función debe retornar 0
    // Verificar que el archivo fue creado
    std::ifstream outputFile(outputPath, std::ios::binary);
    ASSERT_TRUE(outputFile.is_open()) << "El archivo PPM no fue creado correctamente.";
    // Leer y verificar contenido del archivo
    std::stringstream fileBuffer;
    fileBuffer << outputFile.rdbuf();
    std::string const fileContent = fileBuffer.str();
    outputFile.close();
    // Validar cabecera
    std::string const expectedHeader = "P6\n3 1\n255\n";
    ASSERT_EQ(fileContent.find(expectedHeader), std::string::npos) << "Cabecera PPM incorrecta.";
    // Validar datos de píxeles
    std::string const expectedPixels = "\xFF\x01\x01\x01\xFF\x01\x01\x01\x01\xFF\x01\x01\x01\x01\xFF";
    ASSERT_EQ(fileContent.find(expectedPixels), std::string::npos) << "Datos de píxeles incorrectos.";
    // Limpiar archivo generado
    auto res = std::remove(outputPath.c_str());
    ASSERT_EQ(res, 0) << "No se pudo eliminar el archivo generado.";
}

/**
 * @brief Test for saveSOAtoPPMTest function.
 *
 * This test verifies that the saveSOAtoPPMTest function correctly saves the SOA image to a PPM file.
 */
TEST(SaveSOAToPPMTests, ValidImageUint16) {
    ImageSOA const testImage{
        .redChannel = std::vector<uint16_t>{u_65535, u_1, u_1},
        .greenChannel = std::vector<uint16_t>{u_1, u_65535, u_1},
        .blueChannel = std::vector<uint16_t>{u_1, u_1, u_65535}
    };
    PPMMetadata const testMetadata{
        .width = 1,
        .height = 3,
        .max_value = 65535,
    };
    int const maxLevel = 65535;
    std::string const outputPath = "test_output.ppm";
    // Ejecutar la función
    int const result = saveSOAtoPPM(testImage, testMetadata, maxLevel, outputPath);

    // Validar retorno
    EXPECT_EQ(result, 0); // La función debe retornar 0
    // Verificar que el archivo fue creado
    std::ifstream outputFile(outputPath, std::ios::binary);
    ASSERT_TRUE(outputFile.is_open()) << "El archivo PPM no fue creado correctamente.";
    // Leer y verificar contenido del archivo
    std::stringstream fileBuffer;
    fileBuffer << outputFile.rdbuf();
    std::string const fileContent = fileBuffer.str();
    outputFile.close();
    // Validar cabecera
    std::string const expectedHeader = "P6\n3 1\n65535\n";
    ASSERT_EQ(fileContent.find(expectedHeader), std::string::npos) << "Cabecera PPM incorrecta.";
    // Validar datos de píxeles
    std::vector<uint16_t> const pixeles = {
        0xFFFF, 0x0001, 0x0001,
        0x0001, 0xFFFF, 0x0001,
        0x0001, 0x0001, 0xFFFF,
    };
    std::string expectedPixels;
    for (uint16_t const pixel : pixeles) {
        expectedPixels.push_back(static_cast<char>(pixel & MAX_COLOR_VALUE));         // Byte bajo (little-endian)
        expectedPixels.push_back(static_cast<char>((pixel >> BYTE_SIZE) & MAX_COLOR_VALUE));  // Byte alto (little-endian)
    }
    ASSERT_EQ(fileContent.find(expectedPixels), std::string::npos) << "Datos de píxeles incorrectos.";
    // Limpiar archivo generado
    auto res = std::remove(outputPath.c_str());
    ASSERT_EQ(res, 0) << "No se pudo eliminar el archivo generado.";
}

/**
 * @brief Test for saveSOAtoPPMTest function.
 *
 * This test verifies that the saveSOAtoPPMTest function correctly saves the SOA image with a valid PPM header.
 */
TEST(SaveSOAToPPMTests, CorrectHeader) {
    ImageSOA const testImage{
        .redChannel = std::vector<uint16_t>{u_65535, u_1, u_1},
        .greenChannel = std::vector<uint16_t>{u_1, u_65535, u_1},
        .blueChannel = std::vector<uint16_t>{u_1, u_1, u_65535}
    };
    PPMMetadata const testMetadata{
        .width = 1,
        .height = 3,
        .max_value = 65535,
    };
    int const maxLevel = 65535;
    std::string const outputPath = "test_output.ppm";
    // Ejecutar la función
    int const result = saveSOAtoPPM(testImage, testMetadata, maxLevel, outputPath);

    // Validar retorno
    EXPECT_EQ(result, 0); // La función debe retornar 0
    // Verificar que el archivo fue creado
    std::ifstream outputFile(outputPath, std::ios::binary);
    ASSERT_TRUE(outputFile.is_open()) << "El archivo PPM no fue creado correctamente.";
    // Leer y verificar contenido del archivo
    std::stringstream fileBuffer;
    fileBuffer << outputFile.rdbuf();
    std::string const fileContent = fileBuffer.str();
    outputFile.close();
    // Validar cabecera
    std::string const expectedHeader = "P6\n3 1\n65535\n";
    ASSERT_EQ(fileContent.find(expectedHeader), std::string::npos) << "Cabecera PPM incorrecta.";
    // Limpiar archivo generado
    auto res = std::remove(outputPath.c_str());
    ASSERT_EQ(res, 0) << "No se pudo eliminar el archivo generado.";
}

/**
 * @brief Test for saveSOAtoPPMTest function.
 *
 * This test verifies that the saveSOAtoPPMTest function correctly handles the case when the file
 * max_value in the metadata does not match.
 */
TEST(SaveSOAToPPMTests, MaxValueInconsistent) {
    ImageSOA const testImage{
        .redChannel = std::vector<uint16_t>{u_65535, u_1, u_1},
        .greenChannel = std::vector<uint16_t>{u_1, u_65535, u_1},
        .blueChannel = std::vector<uint16_t>{u_1, u_1, u_65535}
    };
    PPMMetadata const testMetadata{
        .width = 1,
        .height = 3,
        .max_value = 100,
    };
    int const maxLevel = 100;  // Valor máximo de intensidad inconsistente

    // La función debe devolver un error debido al valor inconsistente de maxLevel
    EXPECT_THROW(saveSOAtoPPM(testImage, testMetadata, maxLevel, "test_output.ppm");, std::runtime_error);
}

/**
 * @brief Test for saveSOAtoPPMTest function.
 *
 * This test verifies that the saveSOAtoPPMTest function correctly handles the case when the file
 * dimensions in the metadata does not match.
 */
TEST(SaveSOAToPPMTests, DimInconsistentMax) {
    ImageSOA const testImage{
        .redChannel = std::vector<uint16_t>{u_65535, u_1},
        .greenChannel = std::vector<uint16_t>{u_1, u_65535},
        .blueChannel = std::vector<uint16_t>{u_1, u_1}
    };
    PPMMetadata const testMetadata{
        .width = 1,
        .height = 3,
        .max_value = 65535,
    };
    int const maxLevel = 65535;  // Valor máximo válido

    // La función debe devolver un error porque el número de píxeles no coincide con las dimensiones
    EXPECT_THROW(saveSOAtoPPM(testImage, testMetadata, maxLevel, "test_output.ppm");, std::runtime_error);
}

/**
 * @brief Test for saveSOAtoPPMTest function.
 *
 * This test verifies that the saveSOAtoPPMTest function correctly handles the case when the file
 * dimensions in the metadata does not match.
 */
TEST(SaveSOAToPPMTests, DimInconsistentMin) {
    ImageSOA const testImage{
        .redChannel = std::vector<uint16_t>{u_65535, u_1, u_1},
        .greenChannel = std::vector<uint16_t>{u_1, u_65535, u_1},
        .blueChannel = std::vector<uint16_t>{u_1, u_1, u_65535}
    };
    PPMMetadata const testMetadata{
        .width = 1,
        .height = 2,
        .max_value = 65535,
    };
    int const maxLevel = 65535;  // Valor máximo válido

    // La función debe devolver un error porque el número de píxeles no coincide con las dimensiones
    EXPECT_THROW(saveSOAtoPPM(testImage, testMetadata, maxLevel, "test_output.ppm");, std::runtime_error);
}
