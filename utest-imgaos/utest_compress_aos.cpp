//
// Created by mapor on 09/11/2024.
//

#include "utest_aos.hpp"
#include "./imgaos/compress.hpp"
#include "./imgaos/compress.cpp"

// Prueba para comparar dos Pixel8
TEST(Pixel8Test, ComparisonOperatorEqual) {
  Pixel8 const pixel1 = {.r=255, .g=0, .b=0}; // Rojo
  Pixel8 const pixel2 = {.r=255, .g=0, .b=0}; // Rojo
  Pixel8 const pixel3 = {.r=0, .g=255, .b=0}; // Verde

  EXPECT_TRUE(pixel1 == pixel2); // Debería ser igual
  EXPECT_FALSE(pixel1 == pixel3); // Debería ser diferente
}

// Prueba para comparar dos Pixel16
TEST(Pixel16Test, ComparisonOperatorEqual) {
  Pixel16 const pixel1 = {.r=65535, .g=0, .b=0}; // Rojo
  Pixel16 const pixel2 = {.r=65535, .g=0, .b=0}; // Rojo
  Pixel16 const pixel3 = {.r=0, .g=65535, .b=0}; // Verde

  EXPECT_TRUE(pixel1 == pixel2); // Debería ser igual
  EXPECT_FALSE(pixel1 == pixel3); // Debería ser diferente
}
TEST(ColorCompareTest, ComparisonOperator) {
  ColorCompare const compare;
  Pixel8 const pixel1 = {.r=255, .g=0, .b=0}; // Rojo
  Pixel8 const pixel2 = {.r=0, .g=255, .b=0}; // Verde
  Pixel8 const pixel3 = {.r=0, .g=0, .b=255}; // Azul
  Pixel8 const pixel4 = {.r=255, .g=255, .b=0}; // Amarillo

  EXPECT_FALSE(compare(pixel1, pixel2)); // p1 < p2
  EXPECT_FALSE(compare(pixel2, pixel3)); // p2 < p3
  EXPECT_TRUE(compare(pixel3, pixel2)); // p3 >= p2
  EXPECT_FALSE(compare(pixel4, pixel1)); // p4 < p1
}
TEST(HashTest, Pixel8Hash) {
  Pixel8 const pixel1 = {.r=255, .g=0, .b=0}; // Rojo
  Pixel8 const pixel2 = {.r=255, .g=0, .b=0}; // Rojo
  Pixel8 const pixel3 = {.r=0, .g=255, .b=0}; // Verde

  std::unordered_set<Pixel8> set;
  set.insert(pixel1);
  set.insert(pixel2);
  set.insert(pixel3);

  EXPECT_EQ(set.size(), 2); // El set debe tener solo 2 elementos únicos
}

TEST(HashTest, Pixel16Hash) {
  Pixel16 const pixel1 = {.r=65535, .g=0, .b=0}; // Rojo
  Pixel16 const pixel2 = {.r=65535, .g=0, .b=0}; // Rojo
  Pixel16 const pixel3 = {.r=0, .g=65535, .b=0}; // Verde

  std::unordered_set<Pixel16> set;
  set.insert(pixel1);
  set.insert(pixel2);
  set.insert(pixel3);

  EXPECT_EQ(set.size(), 2); // El set debe tener solo 2 elementos únicos
}

TEST(IdentifyUniqueColorsTest, Pixel8) {
  std::vector<Pixel8> const pixels = {{.r=255, .g=0, .b=0}, {.r=0, .g=255, .b=0}, {.r=255, .g=0, .b=0}, {.r=0, .g=0, .b=255}};
  std::unordered_map<Pixel8, uint8_t> pixelToIndex;
  std::set<Pixel8, ColorCompare> colorTable;

  identifyUniqueColors(pixels, pixelToIndex, colorTable);

  EXPECT_EQ(colorTable.size(), 3); // Debe haber 3 colores únicos
  EXPECT_EQ(pixelToIndex.at({255, 0, 0}), 2); // El índice del color rojo debe ser 2
  EXPECT_EQ(pixelToIndex.at({0, 255, 0}), 1); // El índice del color verde debe ser 1
  EXPECT_EQ(pixelToIndex.at({0, 0, 255}), 0); // El índice del color azul debe ser 0
}

TEST(IdentifyUniqueColorsTest, Pixel16) {
  std::vector<Pixel16> const pixels = {{.r=65535, .g=0, .b=0}, {.r=0, .g=65535, .b=0}, {.r=65535, .g=0, .b=0}, {.r=0, .g=0, .b=65535}};
  std::unordered_map<Pixel16, uint16_t> pixelToIndex;
  std::set<Pixel16, ColorCompare> colorTable;

  identifyUniqueColors(pixels, pixelToIndex, colorTable);

  EXPECT_EQ(colorTable.size(), 3); // Debe haber 3 colores únicos
  EXPECT_EQ(pixelToIndex.at({65535, 0, 0}), 2); // El índice del color rojo debe ser 0
  EXPECT_EQ(pixelToIndex.at({0, 65535, 0}), 1); // El índice del color verde debe ser 1
  EXPECT_EQ(pixelToIndex.at({0, 0, 65535}), 0); // El índice del color azul debe ser 2
}

TEST(PopulateColorAndPixelBufferTest, Pixel8) {
  std::vector<Pixel8> pixels = {{.r=n_255, .g=0, .b=0}, {.r=0, .g=n_255, .b=0}, {.r=n_255, .g=0, .b=0}};
  std::unordered_map<Pixel8, uint8_t> const pixelToIndex = {{pixels[0], 0}, {pixels[1], 1}};
  std::set<Pixel8, ColorCompare> const colorTable = {pixels[0], pixels[1]};
  std::vector<uint8_t> buffer;

  populateColorAndPixelBuffer(pixels, pixelToIndex, colorTable, buffer);

  EXPECT_EQ(buffer.size(), 6 + 3); // 6 bytes para los colores, 3 bytes para los índices
  EXPECT_EQ(buffer[0], 0); //
  EXPECT_EQ(buffer[1], 255); //
  EXPECT_EQ(buffer[2], 0); //
  EXPECT_EQ(buffer[3], 255); //
  EXPECT_EQ(buffer[4], 0); //
  EXPECT_EQ(buffer[5], 0); //
  EXPECT_EQ(buffer[6], 0); // Índice del primer píxel
  EXPECT_EQ(buffer[7], 1); // Índice del segundo píxel
}
TEST(PopulateColorAndPixelBufferTest, Pixel16) {
  // Definir algunos colores de 16 bits
  std::vector<Pixel16> pixels = {
      {.r = n_65535, .g = 0, .b = 0},   // Rojo
      {.r = 0, .g = n_65535, .b = 0},   // Verde
      {.r = n_65535, .g = 0, .b = 0}};  // Rojo (duplicado)

  // Crear un map de los píxeles a índices
  std::unordered_map<Pixel16, uint16_t> pixelToIndex;
  pixelToIndex[pixels[0]] = 0; // Primer píxel (Rojo)
  pixelToIndex[pixels[1]] = 1; // Segundo píxel (Verde)
  pixelToIndex[pixels[2]] = 0; // Tercer píxel (Rojo duplicado)

  // Usar ColorCompare para ordenar los colores (esto asegura que el color verde se coloque antes que el rojo)
  std::set<Pixel16, ColorCompare> const colorTable = {pixels[0], pixels[1]};

  // Buffer donde se almacenarán los resultados
  std::vector<uint8_t> buffer;

  // Llamada a la función para poblar el buffer
  populateColorAndPixelBuffer(pixels, pixelToIndex, colorTable, buffer);

  // Verificar el tamaño del buffer (debe contener 12 bytes para colores + 6 bytes para los índices)
  EXPECT_EQ(buffer.size(), 12 + 6); // 12 bytes para los colores, 6 bytes para los índices

  // Los colores deben estar en orden: Verde, Rojo (debido a ColorCompare)

  // **Verde**: R=0, G=65535, B=0
  EXPECT_EQ(buffer[0], 0);         // Verde - R (0)
  EXPECT_EQ(buffer[1], 0);         // Verde - R (0)
  EXPECT_EQ(buffer[2], 255);       // Verde - G (65535 / 256 = 255)
  EXPECT_EQ(buffer[3], 255);       // Verde - G (65535 / 256 = 255)
  EXPECT_EQ(buffer[4], 0);         // Verde - B (0)
  EXPECT_EQ(buffer[5], 0);         // Verde - B (0)

  // **Rojo**: R=65535, G=0, B=0
  EXPECT_EQ(buffer[6], 255);       // Rojo - R (65535 / 256 = 255)
  EXPECT_EQ(buffer[7], 255);       // Rojo - R (65535 / 256 = 255)
  EXPECT_EQ(buffer[8], 0);         // Rojo - G (0)
  EXPECT_EQ(buffer[9], 0);         // Rojo - G (0)
  EXPECT_EQ(buffer[10], 0);        // Rojo - B (0)
  EXPECT_EQ(buffer[11], 0);        // Rojo - B (0)

  // Índice del primer píxel (Rojo) -> 0
  EXPECT_EQ(buffer[12], 0);        // Índice del primer píxel (Rojo) - Byte alto (0)
  EXPECT_EQ(buffer[13], 0);        // Índice del primer píxel (Rojo) - Byte bajo (0)

  // Índice del segundo píxel (Verde) -> 1
  EXPECT_EQ(buffer[14], 0);        // Índice del segundo píxel (Verde) - Byte alto (0)
  EXPECT_EQ(buffer[15], 1);        // Índice del segundo píxel (Verde) - Byte bajo (1)

  // Índice del tercer píxel (Rojo duplicado) -> 0
  EXPECT_EQ(buffer[16], 0);        // Índice del tercer píxel (Rojo duplicado) - Byte alto (0)
  EXPECT_EQ(buffer[17], 0);        // Índice del tercer píxel (Rojo duplicado) - Byte bajo (0)
}



TEST(WriteCPPMTest, Basic) {
  ImageAOS const image = {std::vector<Pixel8>{{.r=255, .g=0, .b=0}, {.r=0, .g=255, .b=0}, {.r=255, .g=0, .b=0}}};
  PPMMetadata const metadata = {.width=3, .height=1, .max_value=255};

  writeCPPM(image, "output.ppm", metadata);

  std::ifstream file("output.ppm", std::ios::binary);
  ASSERT_TRUE(file.is_open());
  std::string const content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  EXPECT_TRUE(content.find("C6") != std::string::npos); // Verificar si se genera el encabezado correcto
  file.close();
}