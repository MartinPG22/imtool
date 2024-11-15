//
// Created by mapor on 13/11/2024.
//
#include "utest_aos.hpp"

// Funciones para obtener los nombres de archivo
std::string getFileName8() {
    return "test_image_8.ppm";
}

std::string getFileName16() {
    return "test_image_16.ppm";
}

std::string getOutputPath() {
    return "output_image.ppm";
}

// Función para crear un archivo PPM de prueba de 8 bits
void crearPPM8Bit(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    file << "P6\n" << kWidth << " " << kHeight << "\n" << kMaxValue8 << "\n";
    file.put(static_cast<char>(n_100))
        .put(static_cast<char>(n_150))
        .put(static_cast<char>(n_200));
    file.put(static_cast<char>(n_50))
        .put(static_cast<char>(n_100))
        .put(static_cast<char>(n_150));
    file.put(static_cast<char>(0))
        .put(static_cast<char>(0))
        .put(static_cast<char>(0));
    file.put(static_cast<char>(n_255))
        .put(static_cast<char>(n_255))
        .put(static_cast<char>(n_255));
    file.close();
}

// Función para crear un archivo PPM de prueba de 16 bits
void crearPPM16Bit(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    file << "P6\n" << kWidth << " " << kHeight << "\n" << kMaxValue16 << "\n";

    // Escribir píxeles en formato de 16 bits
    file.put(static_cast<char>(0)).put(static_cast<char>(n_100));  // R
    file.put(static_cast<char>(0)).put(static_cast<char>(n_150));  // G
    file.put(static_cast<char>(0)).put(static_cast<char>(n_200));  // B

    file.put(static_cast<char>(0)).put(static_cast<char>(n_50));   // R
    file.put(static_cast<char>(0)).put(static_cast<char>(n_100));  // G
    file.put(static_cast<char>(0)).put(static_cast<char>(n_150));  // B

    file.put(static_cast<char>(0)).put(static_cast<char>(0));  // R
    file.put(static_cast<char>(0)).put(static_cast<char>(0));  // G
    file.put(static_cast<char>(0)).put(static_cast<char>(0));  // B

    file.put(static_cast<char>(n_0xFF)).put(static_cast<char>(n_0xFF));  // R
    file.put(static_cast<char>(n_0xFF)).put(static_cast<char>(n_0xFF));  // G
    file.put(static_cast<char>(n_0xFF)).put(static_cast<char>(n_0xFF));  // B

    file.close();
}

// Configuración de los archivos de prueba
void SetUpTestFiles() {
    crearPPM8Bit(getFileName8());
    crearPPM16Bit(getFileName16());
}

void TearDownTestFiles() {
    if (std::remove(getFileName8().c_str()) != 0) {
        std::cerr << "Warning: Failed to remove file " << getFileName8() << '\n';
    }
    if (std::remove(getFileName16().c_str()) != 0) {
        std::cerr << "Warning: Failed to remove file " << getFileName16() << '\n';
    }
    if (std::remove(getOutputPath().c_str()) != 0) {
        std::cerr << "Warning: Failed to remove file " << getOutputPath() << '\n';
    }
}
// Prueba de cargarPixels8 para cargar correctamente un archivo PPM de 8 bits
TEST(CargarPixelsTest, cargarPixels8CargaCorrectamente) {
    SetUpTestFiles();

    std::ifstream archivo(getFileName8(), std::ios::binary);

    // Leer y mostrar el encabezado
    std::string magicNumber;
    archivo >> magicNumber;  // Leer "P6"
    ASSERT_EQ(magicNumber, "P6");
    std::cerr << "Magic Number: " << magicNumber << '\n';  // Verificar que es "P6"

    PPMMetadata metadata{};
    archivo >> metadata.width >> metadata.height >> metadata.max_value;

    archivo.ignore();  // Ignorar el salto de línea después del encabezado

    // Leer las primeras tres líneas para ver cómo están siendo leídas
    archivo.clear();  // Limpiar cualquier estado de error si es que ha habido uno
    archivo.seekg(0, std::ios::beg);  // Ir al principio del archivo
    std::string line;
    for (int i = 0; i < 3; ++i) {
        std::getline(archivo, line);
    }

    // Leer los píxeles
    ImageAOS imagen = cargarPixels8(archivo, metadata.width * metadata.height);

    // Verificar que los píxeles se cargaron correctamente
    ASSERT_TRUE(std::holds_alternative<std::vector<Pixel8>>(imagen.pixels));
    const auto& pixels8 = std::get<std::vector<Pixel8>>(imagen.pixels);
    ASSERT_EQ(pixels8.size(), 4);  // Debe haber 4 píxeles en la imagen de prueba

    // Comprobamos que los valores de los píxeles son correctos
    EXPECT_EQ(pixels8[0].r, 100);
    EXPECT_EQ(pixels8[0].g, 150);
    EXPECT_EQ(pixels8[0].b, 200);
    EXPECT_EQ(pixels8[3].r, 255);
    EXPECT_EQ(pixels8[3].g, 255);
    EXPECT_EQ(pixels8[3].b, 255);

    archivo.close();
}
// Función auxiliar para leer el encabezado PPM
bool leerEncabezadoPPM(std::ifstream& archivo, PPMMetadata& metadata) {
    std::string magicNumber;
    archivo >> magicNumber;
    if (magicNumber != "P6") {
        return false; // No es un archivo PPM válido
    }

    archivo >> metadata.width >> metadata.height >> metadata.max_value;
    archivo.ignore();  // Ignorar el salto de línea después del encabezado
    return true;
}
// Función auxiliar para validar los valores de los píxeles
void validarPixeles16(const std::vector<Pixel16>& pixels16) {
    // Verificar los valores de los píxeles leídos
    EXPECT_EQ(pixels16[0].r, 25600);
    EXPECT_EQ(pixels16[0].g, 38400);
    EXPECT_EQ(pixels16[0].b, 51200);

    EXPECT_EQ(pixels16[1].r, 12800);
    EXPECT_EQ(pixels16[1].g, 25600);
    EXPECT_EQ(pixels16[1].b, 38400);

    EXPECT_EQ(pixels16[2].r, 0);
    EXPECT_EQ(pixels16[2].g, 0);
    EXPECT_EQ(pixels16[2].b, 0);

    EXPECT_EQ(pixels16[3].r, 65535);
    EXPECT_EQ(pixels16[3].g, 65535);
    EXPECT_EQ(pixels16[3].b, 65535);
}
// Test refactorizado
TEST(CargarPixelsTest, cargarPixels16CargaCorrectamente) {
    SetUpTestFiles();

    // Abrir el archivo de 16 bits para leer
    std::ifstream archivo(getFileName16(), std::ios::binary);

    // Leer el encabezado
    PPMMetadata metadata{};
    ASSERT_TRUE(leerEncabezadoPPM(archivo, metadata));

    // Leer las primeras tres líneas para ver cómo están siendo leídas
    archivo.clear();
    archivo.seekg(0, std::ios::beg);
    for (int i = 0; i < 3; ++i) {
        std::string line;
        std::getline(archivo, line);
    }

    // Llamar a la función cargarPixels16 para leer los píxeles
    ImageAOS imagen = cargarPixels16(archivo, metadata.width * metadata.height);

    // Verificar que la imagen contiene píxeles de tipo Pixel16
    ASSERT_TRUE(std::holds_alternative<std::vector<Pixel16>>(imagen.pixels));

    // Obtener los píxeles de la imagen cargada
    const auto& pixels16 = std::get<std::vector<Pixel16>>(imagen.pixels);

    // Comprobar que se han leído 4 píxeles (en este caso de 2x2)
    ASSERT_EQ(pixels16.size(), 4);

    // Validar los valores de los píxeles
    validarPixeles16(pixels16);

    archivo.close();
}

// Prueba de cargarImagenPPMAOS para leer correctamente archivos PPM en formato AOS
TEST(CargarImagenPPMAOSTest, cargarImagenPPMAOSCargaCorrectamente) {
    SetUpTestFiles();

    PPMMetadata metadata{};
    ImageAOS const imagen8 = cargarImagenPPMAOS(getFileName8(), metadata);
    ASSERT_TRUE(std::holds_alternative<std::vector<Pixel8>>(imagen8.pixels));

    metadata = {};
    ImageAOS const imagen16 = cargarImagenPPMAOS(getFileName16(), metadata);
    ASSERT_TRUE(std::holds_alternative<std::vector<Pixel16>>(imagen16.pixels));

    TearDownTestFiles();
}

// Prueba de saveAOStoPPM para guardar correctamente una imagen en formato PPM
TEST(SaveImageTest, saveAOStoPPMGuardaCorrectamente) {
    SetUpTestFiles();

    PPMMetadata metadata = {.width=kWidth, .height=kHeight, .max_value=kMaxValue8};
    ImageAOS const imagen = cargarImagenPPMAOS(getFileName8(), metadata);
    ASSERT_EQ(saveAOStoPPM(imagen, metadata, kMaxValue8, getOutputPath()), 0);

    std::ifstream outFile(getOutputPath(), std::ios::binary);
    ASSERT_TRUE(outFile.is_open());

    std::string header;
    std::getline(outFile, header); // Leer encabezado P6
    EXPECT_EQ(header, "P6");
    int width = 0;
    int height = 0;
    int max_value = 0;
    outFile >> width >> height >> max_value;
    EXPECT_EQ(width, kWidth);
    EXPECT_EQ(height, kHeight);
    EXPECT_EQ(max_value, kMaxValue8);

    outFile.close();
    TearDownTestFiles();
}