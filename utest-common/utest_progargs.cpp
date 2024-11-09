//
// Created by mapor on 09/11/2024.
//
#include "utest_common.hpp"

TEST(IsIntegerTest, ValidInteger) {
  EXPECT_TRUE(isInteger("123"));
  EXPECT_TRUE(isInteger("-123"));
  EXPECT_TRUE(isInteger("0"));
}

TEST(IsIntegerTest, InvalidInteger) {
  EXPECT_FALSE(isInteger("123.45"));
  EXPECT_FALSE(isInteger("abc"));
  EXPECT_FALSE(isInteger(""));
}

// Caso de prueba para la función executeInfo
TEST(ExecuteInfoTest, ValidArguments) {
    // Configuración de los argumentos y metadatos para el caso de prueba
    std::vector<std::string> const arguments = {"input.ppm", "output.ppm", "info"};
    PPMMetadata const metadata = {.width=800, .height=600, .max_value=255};

    // Llamada a la función a probar
    int const result = executeInfo(arguments, metadata);

    // Verificación del resultado esperado
    EXPECT_EQ(result, 0);
}

TEST(ExecuteMaxlevelTest, ValidMaxLevel) {
    // Configuración de los argumentos y metadatos para el caso de prueba
    std::vector<std::string> const arguments = {"input.ppm", "output.ppm", "maxlevel", "128"};
    PPMMetadata metadata = {.width=n_800, .height=n_600, .max_value=n_255};
    std::string const method = "aos";

    // Llamada a la función a probar
    int const result = executeMaxlevel(arguments, metadata, method);

    // Verificación del resultado esperado
    EXPECT_EQ(result, 0);
}

TEST(ExecuteResizeTest, ValidResizeArguments) {
    // Configuración de los argumentos y metadatos para el caso de prueba
    std::vector<std::string> const arguments = {"input.ppm", "output.ppm", "resize", "400", "300"};
    PPMMetadata metadata = {.width=n_800, .height=n_600, .max_value=n_255};
    std::string const method = "soa";

    // Llamada a la función a probar
    int const result = executeResize(arguments, metadata, method);

    // Verificación del resultado esperado
    EXPECT_EQ(result, 0);
}

TEST(ExecuteCutfreqTest, ValidCutfreqArguments) {
    // Configuración de los argumentos y metadatos para el caso de prueba
    std::vector<std::string> const arguments = {"input.ppm", "output.ppm", "cutfreq", "5"};
    PPMMetadata metadata = {.width=n_800, .height=n_600, .max_value=n_255};
    std::string const method = "aos";

    // Llamada a la función a probar
    int const result = executeCutfreq(arguments, metadata, method);

    // Verificación del resultado esperado
    EXPECT_EQ(result, 0);
}

TEST(ExecuteCompressTest, ValidCompressArguments) {
    // Configuración de los argumentos y metadatos para el caso de prueba
    std::vector<std::string> const arguments = {"input.ppm", "output.ppm", "compress"};
    PPMMetadata metadata = {.width=n_800, .height=n_600, .max_value=n_255};
    std::string const method = "soa";

    // Llamada a la función a probar
    int const result = executeCompress(arguments, metadata, method);

    // Verificación del resultado esperado
    EXPECT_EQ(result, 0);
}

// ------------------------------------------------------------------
// Caso de prueba para la función executeInfo
TEST(ExecuteInfoTest, ValidArguments2) {
    // Configuración de los argumentos y metadatos para el caso de prueba
    std::vector<std::string> const arguments = {"input.ppm", "output.ppm", "info"};
    PPMMetadata const metadata = {.width=800, .height=600, .max_value=255};

    // Llamada a la función a probar
    int const result = executeInfo(arguments, metadata);

    // Verificación del resultado esperado
    EXPECT_EQ(result, 0);
}

TEST(ExecuteMaxlevelTest, ValidMaxLevel2) {
    // Configuración de los argumentos y metadatos para el caso de prueba
    std::vector<std::string> const arguments = {"input.ppm", "output.ppm", "maxlevel", "128"};
    PPMMetadata metadata = {.width=n_800, .height=n_600, .max_value=n_255};
    std::string const method = "aos";

    // Llamada a la función a probar
    int const result = executeMaxlevel(arguments, metadata, method);

    // Verificación del resultado esperado
    EXPECT_EQ(result, 0);
}

TEST(ExecuteResizeTest, ValidResizeArguments2) {
    // Configuración de los argumentos y metadatos para el caso de prueba
    std::vector<std::string> const arguments = {"input.ppm", "output.ppm", "resize", "400", "300"};
    PPMMetadata metadata = {.width=n_800, .height=n_600, .max_value=n_255};
    std::string const method = "soa";

    // Llamada a la función a probar
    int const result = executeResize(arguments, metadata, method);

    // Verificación del resultado esperado
    EXPECT_EQ(result, 0);
}

TEST(ExecuteCutfreqTest, ValidCutfreqArguments2) {
    // Configuración de los argumentos y metadatos para el caso de prueba
    std::vector<std::string> const arguments = {"input.ppm", "output.ppm", "cutfreq", "5"};
    PPMMetadata metadata = {.width=n_800, .height=n_600, .max_value=n_255};
    std::string const method = "aos";

    // Llamada a la función a probar
    int const result = executeCutfreq(arguments, metadata, method);

    // Verificación del resultado esperado
    EXPECT_EQ(result, 0);
}

TEST(ExecuteCompressTest, ValidCompressArguments2) {
    // Configuración de los argumentos y metadatos para el caso de prueba
    std::vector<std::string> const arguments = {"input.ppm", "output.ppm", "compress"};
    PPMMetadata metadata = {.width=n_800, .height=n_600, .max_value=n_255};
    std::string const method = "soa";

    // Llamada a la función a probar
    int const result = executeCompress(arguments, metadata, method);

    // Verificación del resultado esperado
    EXPECT_EQ(result, 0);
}