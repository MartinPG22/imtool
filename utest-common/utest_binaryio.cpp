//
// Created by mapor on 09/11/2024.
//

#include "utest_common.hpp"

// Test para la función readBinaryFile
TEST(ReadBinaryFileTest, ValidFile) {
  // Crear un archivo de prueba
  const std::string filename = "test_file.bin";
  std::ofstream outFile(filename, std::ios::binary);
  outFile.write("Hello, world!", n_13);
  outFile.close();

  // Leer el archivo
  std::vector<uint8_t> result = readBinaryFile(filename);
  EXPECT_EQ(result.size(), 13);  // Debe tener 13 bytes
  EXPECT_EQ(result[0], 'H');     // El primer byte debe ser 'H'
  EXPECT_EQ(result[1], 'e');     // El segundo byte debe ser 'e'

  // Limpiar el archivo de prueba
  if (std::remove(filename.c_str()) != 0) {
    std::cerr << "Error al eliminar el archivo: " << filename << '\n';
    // Opcional: Lanzar una excepción o manejar el error de otra manera si lo necesitas
  }
}

TEST(ReadBinaryFileTest, FileNotFound) {
  // Intentar abrir un archivo que no existe
  EXPECT_THROW(readBinaryFile("nonexistent_file.bin"), std::runtime_error);
}

TEST(ReadBinaryFileTest, EmptyFile) {
  const std::string filename = "empty_file.bin";
  std::ofstream outFile(filename, std::ios::binary);
  outFile.close();  // Crear un archivo vacío

  std::vector<uint8_t> const result = readBinaryFile(filename);
  EXPECT_TRUE(result.empty());  // Debe estar vacío

  // Limpiar el archivo de prueba
  if (std::remove(filename.c_str()) != 0) {
    std::cerr << "Error al eliminar el archivo: " << filename << '\n';
    // Opcional: Lanzar una excepción o manejar el error de otra manera si lo necesitas
  }
}

// Test para la función readLine
TEST(ReadLineTest, ReadLine) {
  std::vector<uint8_t> const data = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '\n', 'E', 'n', 'd'};
  size_t index = 0;

  std::string const line = readLine(data, index);
  EXPECT_EQ(line, "Hello World");  // Línea leída

  // Verificar el índice después de leer
  EXPECT_EQ(index, 12);  // El índice debe ser 13 (después de la línea '\n')
}

TEST(ReadLineTest, LineWithMultipleSpaces) {
  std::vector<uint8_t> const data = {'T', 'e', 's', 't', ' ', 'L', 'i', 'n', 'e', '\n'};
  size_t index = 0;

  std::string const line = readLine(data, index);
  EXPECT_EQ(line, "Test Line");  // Línea leída

  // Verificar el índice después de leer
  EXPECT_EQ(index, 10);  // El índice debe ser 10 (después de la línea '\n')
}

// Test para la función readNextInt
TEST(ReadNextIntTest, ReadValidInt) {
  std::vector<uint8_t> const data = {'1', '2', '3', ' ', '4', '5', '6', '\n'};
  size_t index = 0;

  int value = readNextInt(data, index, "Error leyendo el número.");
  EXPECT_EQ(value, 123);  // El primer número leído debe ser 123
  EXPECT_EQ(index, 3);    // El índice debe estar después de 123

  // Leer el siguiente número
  value = readNextInt(data, index, "Error leyendo el número.");
  EXPECT_EQ(value, 456);  // El siguiente número debe ser 456
  EXPECT_EQ(index, 7);    // El índice debe estar después de 456
}

TEST(ReadNextIntTest, InvalidInt) {
  std::vector<uint8_t> const data = {'a', 'b', 'c', ' ', 'd', 'e', 'f'};
  size_t index = 0;

  // Debe lanzar una excepción de tipo std::invalid_argument porque no es un número
  EXPECT_THROW(readNextInt(data, index, "Error leyendo el número."), std::invalid_argument);
}

// Test para la función readPPMMetadata
TEST(ReadPPMMetadataTest, ValidPPM) {
  const std::string filename = "valid_image.ppm";

  // Crear un archivo PPM de prueba (simplificado)
  std::ofstream outFile(filename, std::ios::binary);
  outFile << "P6\n4 4\n255\n";  // Cabecera PPM
  outFile.write("\0\0\0\0\0\0\0\0", n_8);  // Datos binarios dummy
  outFile.close();

  PPMMetadata const metadata = readPPMMetadata(filename);
  EXPECT_EQ(metadata.width, 4);     // Ancho de la imagen
  EXPECT_EQ(metadata.height, 4);    // Alto de la imagen
  EXPECT_EQ(metadata.max_value, 255); // Máximo valor de color

  // Limpiar el archivo de prueba
  if (std::remove(filename.c_str()) != 0) {
    std::cerr << "Error al eliminar el archivo: " << filename << '\n';
    // Opcional: Lanzar una excepción o manejar el error de otra manera si lo necesitas
  }
}

TEST(ReadPPMMetadataTest, InvalidPPMFormat) {
  const std::string filename = "invalid_format.ppm";

  // Crear un archivo PPM con formato incorrecto
  std::ofstream outFile(filename, std::ios::binary);
  outFile << "P5\n4 4\n255\n";  // Formato incorrecto (debe ser P6)
  outFile.close();

  // Debe lanzar una excepción por formato incorrecto
  EXPECT_THROW(readPPMMetadata(filename), std::runtime_error);

  // Limpiar el archivo de prueba
  if (std::remove(filename.c_str()) != 0) {
    std::cerr << "Error al eliminar el archivo: " << filename << '\n';
    // Opcional: Lanzar una excepción o manejar el error de otra manera si lo necesitas
  }
}

TEST(ReadPPMMetadataTest, InvalidMaxValue) {
  const std::string filename = "invalid_max_value.ppm";

  // Crear un archivo PPM con valor máximo fuera de rango
  std::ofstream outFile(filename, std::ios::binary);
  outFile << "P6\n4 4\n70000\n";  // Valor máximo fuera de rango
  outFile.close();

  // Debe lanzar una excepción por valor máximo fuera de rango
  EXPECT_THROW(readPPMMetadata(filename), std::runtime_error);

  // Limpiar el archivo de prueba
  if (std::remove(filename.c_str()) != 0) {
    std::cerr << "Error al eliminar el archivo: " << filename << '\n';
    // Opcional: Lanzar una excepción o manejar el error de otra manera si lo necesitas
  }
}