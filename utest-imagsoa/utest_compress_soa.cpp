//
// Created by mapor on 09/11/2024.
//

#include "utest_soa.hpp"
#include "./imgsoa/compress.hpp"
#include "./imgsoa/compress.cpp"

TEST(DeterminePixelSizeTest, HandlesPixelSize) {

  // Casos con diferentes números de colores
  EXPECT_EQ(determinePixelSize(100), 1);        // 100 colores => 1 byte
  EXPECT_EQ(determinePixelSize(256), 1);        // 256 colores => 1 byte
  EXPECT_EQ(determinePixelSize(5000), 2);       // 5000 colores => 2 bytes
  EXPECT_EQ(determinePixelSize(65536), 4);      // 65536 colores => 2 bytes
  EXPECT_EQ(determinePixelSize(70000), 4);      // 70000 colores => 4 bytes
}
// Test unitario para la función writeBytes
// Test unitario para la función writeBytes
TEST(WriteBytesTest, WritesBytesCorrectly) {
  // Crear un archivo de prueba
  std::ofstream testFile("test_bytes.bin", std::ios::binary);
  ASSERT_TRUE(testFile.is_open()) << "No se pudo abrir el archivo para escribir.";

  // Crear un buffer con datos de prueba
  std::vector<uint8_t> buffer = {n_65, n_66, n_67, n_68, n_69};  // ASCII de 'A', 'B', 'C', 'D', 'E'

  // Escribir los bytes en el archivo
  writeBytes(testFile, buffer);

  // Cerrar el archivo después de escribir
  testFile.close();

  // Reabrir el archivo para leer los datos escritos
  std::ifstream inputFile("test_bytes.bin", std::ios::binary);
  ASSERT_TRUE(inputFile.is_open()) << "No se pudo abrir el archivo para leer.";

  // Leer los bytes en un vector de char y luego copiar a uint8_t
  std::vector<char> tempBuffer(buffer.size());
  inputFile.read(tempBuffer.data(), static_cast<std::streamsize>(tempBuffer.size()));

  // Convertir `tempBuffer` a `std::vector<uint8_t>`
  std::vector<uint8_t> readBuffer(tempBuffer.begin(), tempBuffer.end());

  // Verificar que el número de bytes leídos sea correcto
  EXPECT_EQ(inputFile.gcount(), static_cast<std::streamsize>(buffer.size()));

  // Verificar que los bytes leídos coincidan con los esperados
  for (size_t i = 0; i < buffer.size(); ++i) {
    EXPECT_EQ(readBuffer[i], buffer[i]) << "Los bytes no coinciden en la posición " << i;
  }

  // Cerrar el archivo después de leer
  inputFile.close();
}

TEST(WriteColorTableTest, WritesColorTable) {

  std::ofstream testFile("test.ppm", std::ios::binary);
  if (!testFile.is_open()) {
    std::cerr << "No se pudo abrir el archivo para escribir." << '\n';
    return;
  }
  std::cout << "Archivo será creado en: " << std::filesystem::current_path() << '\n';
  std::vector<std::tuple<int, int, int>> const colorList = {
    {255, 0, 0},
    {0, 255, 0},
    {0, 0, 255},
  };

  size_t const colorBytes = 3;  // Asumimos que estamos trabajando con 8 bits por componente

  writeColorTable(testFile, colorList, colorBytes);
  testFile.close();

  // Reabrimos el archivo para verificar la escritura
  std::ifstream file("test.ppm", std::ios::binary);
  ASSERT_TRUE(file.is_open());

  // Leemos los primeros 9 bytes (3 colores de 3 bytes)
  std::array<char, n_9> buffer{};  // Usamos std::array en lugar de C-style array
  // Leemos los bytes
  file.read(buffer.data(), n_9);  // No es necesario realizar cast explícito aquí

  // Imprimir el contenido del buffer
  std::cout << "Contenido del buffer leído: ";
  for (const char& byte : buffer) {
    std::cout << static_cast<int>(byte) << " ";  // Convertir a entero para ver el valor numérico
  }
  std::cout << '\n';

  // Comparaciones convertidas a `uint8_t` para evitar el problema de signo
  EXPECT_EQ(static_cast<uint8_t>(buffer[0]), 255); // R de rojo
  EXPECT_EQ(static_cast<uint8_t>(buffer[1]), 0);   // G de rojo
  EXPECT_EQ(static_cast<uint8_t>(buffer[2]), 0);   // B de rojo
  EXPECT_EQ(static_cast<uint8_t>(buffer[3]), 0);   // R de verde
  EXPECT_EQ(static_cast<uint8_t>(buffer[4]), 255); // G de verde
  EXPECT_EQ(static_cast<uint8_t>(buffer[5]), 0);   // B de verde
  EXPECT_EQ(static_cast<uint8_t>(buffer[6]), 0);   // R de azul
  EXPECT_EQ(static_cast<uint8_t>(buffer[7]), 0);   // G de azul
  EXPECT_EQ(static_cast<uint8_t>(buffer[8]), 255); // B de azul


  file.close();
}

// Test de ejemplo para writePixelData
TEST(WritePixelDataTest, WritesPixelDataCorrectly) {
  // Crear un archivo de prueba
  std::ofstream testFile("test_pixels.bin", std::ios::binary);
  ASSERT_TRUE(testFile.is_open()) << "No se pudo abrir el archivo para escribir.";

  // Simulación de la estructura de PixelData
  PixelData pixelData;
  pixelData.redChannel = {n_255, 0, 0};
  pixelData.greenChannel = {0, n_255, 0};
  pixelData.blueChannel = {0, 0, n_255};

  // Mapa de colores a índices para simular una tabla de colores
  pixelData.colorTable = {
    {std::make_tuple(n_255, 0, 0), n_0x000000FF},
    {std::make_tuple(0, n_255, 0), n_0x0000FF00},
    {std::make_tuple(0, 0, n_255), n_0x00FF0000}
  };

  size_t const pixelSize = 1;  // Usar un tamaño de píxel de 1 byte
  size_t const numPixels = pixelData.redChannel.size();  // Número de píxeles

  // Escribir los datos de píxel en el archivo
  writePixelData(testFile, pixelData, pixelSize);
  testFile.close();

  // Reabrir el archivo para leer los datos escritos
  std::ifstream file("test_pixels.bin", std::ios::binary);
  ASSERT_TRUE(file.is_open());

  // Leer los datos en un buffer de tipo `char`
  std::vector<char> charBuffer(numPixels * pixelSize);
  // Verificar el tamaño antes de convertir a std::streamsize
  assert(charBuffer.size() <= static_cast<size_t>(std::numeric_limits<std::streamsize>::max()) &&
       "El tamaño del buffer excede el límite de std::streamsize");

  // Realizar la lectura
  file.read(charBuffer.data(), static_cast<std::streamsize>(charBuffer.size()));

  // Convertir `charBuffer` a `uint8_t` para verificar los valores correctamente
  std::vector<uint8_t> buffer(charBuffer.begin(), charBuffer.end());


  // Imprimir el contenido del buffer leído
  std::cout << "Contenido del buffer leído: ";
  for (const auto& byte : buffer) {
    std::cout << static_cast<int>(byte) << " ";
  }
  std::cout << '\n';

  // Verificar los valores del buffer
  ASSERT_EQ(buffer[0], 0xFF);  // Primer píxel (rojo)
  ASSERT_EQ(buffer[1], 0x00);  // Segundo píxel (verde)
  ASSERT_EQ(buffer[2], 0x00);  // Tercer píxel (azul)

  file.close();
}

// Test unitario para prepareColorChannels
TEST(PrepareColorChannelsTest, Handles8BitColorChannels) {
    // Datos de prueba: imagen con 8 bits por canal
    ImageSOA image;
    image.redChannel = std::vector<uint8_t>{n_255, 0, 0}; // Rojos
    image.greenChannel = std::vector<uint8_t>{0, n_255, 0}; // Verdes
    image.blueChannel = std::vector<uint8_t>{0, 0, n_255}; // Azules

    std::unordered_map<std::tuple<int, int, int>, uint32_t, TupleHash> colorTable;

    // Llamada a la función a probar
    PixelData const pixelData = prepareColorChannels(image, colorTable);

    // Comprobamos que los canales tengan los valores correctos
    ASSERT_EQ(pixelData.redChannel.size(), 3);
    ASSERT_EQ(pixelData.greenChannel.size(), 3);
    ASSERT_EQ(pixelData.blueChannel.size(), 3);

    // Verificar que la tabla de colores tenga 3 colores únicos
    ASSERT_EQ(colorTable.size(), 3);

    // Comprobar que la tabla de colores contiene las entradas correctas
    EXPECT_EQ(colorTable[std::make_tuple(255, 0, 0)], 0);  // Primer color
    EXPECT_EQ(colorTable[std::make_tuple(0, 255, 0)], 1);  // Segundo color
    EXPECT_EQ(colorTable[std::make_tuple(0, 0, 255)], 2);  // Tercer color
}

TEST(PrepareColorChannelsTest, Handles16BitColorChannels) {
    // Datos de prueba: imagen con 16 bits por canal
    ImageSOA image;
    image.redChannel = std::vector<uint16_t>{n_65535, 0, 0}; // Rojos
    image.greenChannel = std::vector<uint16_t>{0, n_65535, 0}; // Verdes
    image.blueChannel = std::vector<uint16_t>{0, 0, n_65535}; // Azules

    std::unordered_map<std::tuple<int, int, int>, uint32_t, TupleHash> colorTable;

    // Llamada a la función a probar
    PixelData const pixelData = prepareColorChannels(image, colorTable);

    // Comprobamos que los canales tengan los valores correctos
    ASSERT_EQ(pixelData.redChannel.size(), 3);
    ASSERT_EQ(pixelData.greenChannel.size(), 3);
    ASSERT_EQ(pixelData.blueChannel.size(), 3);

    // Verificar que la tabla de colores tenga 3 colores únicos
    ASSERT_EQ(colorTable.size(), 3);

    // Comprobar que la tabla de colores contiene las entradas correctas
    EXPECT_EQ(colorTable[std::make_tuple(255, 0, 0)], 0);  // Primer color
    EXPECT_EQ(colorTable[std::make_tuple(0, 255, 0)], 1);  // Segundo color
    EXPECT_EQ(colorTable[std::make_tuple(0, 0, 255)], 2);  // Tercer color
}

TEST(WriteCPPMSOATest, WritesImageCorrectly) {
    // Crear una imagen de prueba (usando datos simples para facilitar la comprobación)
    ImageSOA image;
    image.redChannel = std::vector<uint8_t>{n_255, 0, 0};
    image.greenChannel = std::vector<uint8_t>{0, n_255, 0};
    image.blueChannel = std::vector<uint8_t>{0, 0, n_255};

    PPMMetadata const metadata = {.width=3, .height=1, .max_value=n_255}; // Imagen 3x1 de píxeles, valor máximo 255

    // Nombre temporal para el archivo
    std::string const filename = "test.ppm";

    // Llamar a la función writeCPPMSOA
    writeCPPMSOA(image, filename, metadata);

    // Verificar que el archivo ha sido creado
    ASSERT_TRUE(std::filesystem::exists(filename)) << "El archivo no fue creado.";

    // Leer el archivo generado
    std::ifstream file(filename, std::ios::binary);
    ASSERT_TRUE(file.is_open()) << "No se pudo abrir el archivo para lectura.";

    // Verificar el encabezado
    std::string header;
    std::getline(file, header);
    EXPECT_EQ(header, "C6 3 1 255 3") << "El encabezado no es correcto.";

  // Leer la tabla de colores (esperando 3 colores)
  std::vector<std::tuple<int, int, int>> colorList(3);
  for (size_t i = 0; i < 3; ++i) {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

    // Leer los bytes de cada componente de color
    char redByte = 0;
    char greenByte = 0;
    char blueByte = 0;

    file.read(&redByte, sizeof(redByte));
    file.read(&greenByte, sizeof(greenByte));
    file.read(&blueByte, sizeof(blueByte));

    // Convertir de char a uint8_t
    red = static_cast<uint8_t>(redByte);
    green = static_cast<uint8_t>(greenByte);
    blue = static_cast<uint8_t>(blueByte);

    // Almacenar el color en la lista
    colorList[i] = std::make_tuple(static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue));
  }

    // Verificar la tabla de colores (debería contener 3 colores)
    EXPECT_EQ(colorList.size(), 3);
    EXPECT_EQ(colorList[0], std::make_tuple(0, 0, 255));
    EXPECT_EQ(colorList[1], std::make_tuple(0, 255, 0));
    EXPECT_EQ(colorList[2], std::make_tuple(255, 0, 0));

    // Leer los píxeles (en este caso los índices de la tabla de colores)
  std::vector<uint8_t> pixelData(3);
  for (size_t i = 0; i < 3; ++i) {
    char pixelByte = 0;  // Usamos char temporal para almacenar el byte leído
    file.read(&pixelByte, sizeof(pixelByte));  // Leer un solo byte

    pixelData[i] = static_cast<uint8_t>(pixelByte);  // Convertir de char a uint8_t
  }

    // Verificar los valores de píxeles (deberían ser los índices de la tabla de colores)
    EXPECT_EQ(pixelData[0], 0); // Índice para el color rojo
    EXPECT_EQ(pixelData[1], 1); // Índice para el color verde
    EXPECT_EQ(pixelData[2], 2); // Índice para el color azul

    // Cerrar el archivo
    file.close();
}