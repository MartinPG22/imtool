//
// Created by mapor on 26/10/2024.
//

#include "imagesoa.hpp"


namespace {
    /** @brief Reads the channels with 8-bit components.
     *
     * This function reads the red, green, and blue channels with 8-bit components from a file.
     *
     * @param archivo The file stream to read the channels from.
     * @param imagen The SOA image to store the channels.
     * @param num_pixels The number of pixels to read.
     * @param max_value The maximum intensity level.
     * @throw std::runtime_error If the pixel value is invalid, the number of pixels is incorrect, or there are extra data in the file.
     */
    void leerCanales8Bits(std::ifstream& archivo, const size_t num_pixels, ImageSOA& imagen, const int max_value) {
        auto& red = std::get<std::vector<uint8_t>>(imagen.redChannel);
        auto& green = std::get<std::vector<uint8_t>>(imagen.greenChannel);
        auto& blue = std::get<std::vector<uint8_t>>(imagen.blueChannel);

        std::array<char, 1> buffer{};
        for (size_t i = 0; i < num_pixels; i++) {
            archivo.read(buffer.data(), 1);
            red[i] = static_cast<uint8_t>(buffer[0]);

            archivo.read(buffer.data(), 1);
            green[i] = static_cast<uint8_t>(buffer[0]);

            archivo.read(buffer.data(), 1);
            blue[i] = static_cast<uint8_t>(buffer[0]);

            if (red[i] > max_value || green[i] > max_value || blue[i] > max_value) {
                throw std::runtime_error("Valor de píxel inválido");
            }
            if (archivo.eof()) {
                throw std::runtime_error("Número de píxeles incorrecto");
            }
        }
        std::cout << archivo.peek() << '\n';
        if (archivo.peek() != EOF) {
            throw std::runtime_error("El archivo contiene más datos de los esperados");
        }
    }
    /** @brief Reads the channels with 16-bit components.
     *
     * This function reads the red, green, and blue channels with 16-bit components from a file.
     *
     * @param archivo The file stream to read the channels from.
     * @param imagen The SOA image to store the channels.
     * @param num_pixels The number of pixels to read.
     * @param max_value The maximum intensity level.
     * @throw std::runtime_error If the pixel value is invalid, the number of pixels is incorrect, or there are extra data in the file.
     */
    void leerCanales16Bits(std::ifstream& archivo, const size_t num_pixels, ImageSOA& imagen, const int max_value) {
        auto& red = std::get<std::vector<uint16_t>>(imagen.redChannel);
        auto& green = std::get<std::vector<uint16_t>>(imagen.greenChannel);
        auto& blue = std::get<std::vector<uint16_t>>(imagen.blueChannel);

        std::array<char, 1> buffer{};
        for (size_t i = 0; i < num_pixels; ++i) {
            archivo.read(buffer.data(), 1);
            const auto r_low = static_cast<uint8_t>(buffer[0]);

            archivo.read(buffer.data(), 1);
            const auto r_high = static_cast<uint8_t>(buffer[0]);

            archivo.read(buffer.data(), 1);
            const auto g_low = static_cast<uint8_t>(buffer[0]);

            archivo.read(buffer.data(), 1);
            const auto g_high = static_cast<uint8_t>(buffer[0]);

            archivo.read(buffer.data(), 1);
            const auto b_low = static_cast<uint8_t>(buffer[0]);

            archivo.read(buffer.data(), 1);
            const auto b_high = static_cast<uint8_t>(buffer[0]);

            red[i] = static_cast<uint16_t>(r_low | (r_high << BYTE_SIZE));
            green[i] = static_cast<uint16_t>(g_low | (g_high << BYTE_SIZE));
            blue[i] = static_cast<uint16_t>(b_low | (b_high << BYTE_SIZE));

            if (red[i] > max_value || green[i] > max_value || blue[i] > max_value) {
                throw std::runtime_error("Valor de píxel inválido");
            }
            if (archivo.eof()) {
                throw std::runtime_error("Número de píxeles incorrecto");
            }
        }
        std::cout << archivo.peek() << '\n';
        if (archivo.peek() != EOF) {
            throw std::runtime_error("El archivo contiene más datos de los esperados");
        }
    }
    // Helper function to write PPM header
    void writePPMHeader(std::ofstream& outFile, const PPMMetadata& metadata, const int maxLevel) {
        outFile << "P6\n" << metadata.width << " " << metadata.height << "\n" << maxLevel << "\n";
    }
    struct ColorChannels8 {
        std::vector<uint8_t> red;
        std::vector<uint8_t> green;
        std::vector<uint8_t> blue;
    };
    // Helper function to write 8-bit pixel data
    void writePixelData8(std::ofstream& outFile, const ColorChannels8& channels, const int maxLevel) {
        for (size_t i = 0; i < channels.red.size(); ++i) {
            if (channels.red[i] > maxLevel || channels.green[i] > maxLevel || channels.blue[i] > maxLevel) {
                throw std::runtime_error("Valor de píxel inválido");
            }
            outFile.put(static_cast<char>(channels.red[i]));
            outFile.put(static_cast<char>(channels.green[i]));
            outFile.put(static_cast<char>(channels.blue[i]));
        }
    }
    struct ColorChannels16 {
        std::vector<uint16_t> red;
        std::vector<uint16_t> green;
        std::vector<uint16_t> blue;
    };
    // Helper function to write 16-bit pixel data
    void writePixelData16(std::ofstream& outFile, const ColorChannels16& channels, const int maxLevel) {
        for (size_t i = 0; i < channels.red.size(); ++i) {
            if (channels.red[i] > maxLevel || channels.green[i] > maxLevel || channels.blue[i] > maxLevel) {
                throw std::runtime_error("Valor de píxel inválido");
            }
            outFile.put(static_cast<char>(channels.red[i]));
            outFile.put(static_cast<char>(channels.green[i]));
            outFile.put(static_cast<char>(channels.blue[i]));
        }
    }
}

/**
 * @brief Loads an image in PPM format to an SOA data structure.
 *
 * This function reads an image in PPM (P6) format and stores it in an SOA image.
 * The function can handle images with 8-bit or 16-bit pixels.
 *
 * @param nombre_archivo The path to the PPM file.
 * @param metadata Metadata of the image.
 * @return ImageSOA The image in SOA format.
 * @throws std::runtime_error If the file cannot be opened, the format is not supported, or the image dimensions are invalid.
 */
ImageSOA cargarImagenPPMtoSOA(const std::string& nombre_archivo, PPMMetadata& metadata) {
    std::ifstream archivo(nombre_archivo, std::ios::binary);
    if (!archivo || metadata.width == 0 || metadata.height == 0) {
        throw std::runtime_error("No se pudo abrir el archivo");
    }
    // Leer el tipo de magia
    std::string magicNumber;
    archivo >> magicNumber;
    if (magicNumber != "P6" || nombre_archivo.substr(nombre_archivo.size() - 4) != ".ppm") {
        throw std::runtime_error("Formato de archivo no soportado");
    }
    // Leer dimensiones y valor máximo
    int width = 0;
    int height = 0;
    int max_value = 0;
    archivo >> width >> height >> max_value;
    archivo.ignore(); // Ignorar el salto de línea después del encabezado
    // Crear la estructura ImageSOA
    ImageSOA imagen;
    if (width < 0 || height < 0 || max_value < 0) {
        throw std::runtime_error("Dimensiones de imagen incorrectas");
    }
    metadata.width = static_cast<size_t>(width);
    metadata.height = static_cast<size_t>(height);
    metadata.max_value = max_value;
    size_t const num_pixels = metadata.width * metadata.height;
    // Leer los píxeles en función del valor máximo
    if (metadata.max_value <= METATADATA_MAX_VALUE) {
        imagen.redChannel = std::vector<uint8_t>(num_pixels);
        imagen.greenChannel = std::vector<uint8_t>(num_pixels);
        imagen.blueChannel = std::vector<uint8_t>(num_pixels);
        leerCanales8Bits(archivo, num_pixels, imagen, metadata.max_value);
    } else {
        // Caso: 2 bytes por componente (RGB) en formato little-endian
        imagen.redChannel = std::vector<uint16_t>(num_pixels);
        imagen.greenChannel = std::vector<uint16_t>(num_pixels);
        imagen.blueChannel = std::vector<uint16_t>(num_pixels);
        leerCanales16Bits(archivo, num_pixels, imagen, metadata.max_value);
    }
    return imagen;
}

/** @brief Saves a SOA image data structure to a PPM file.
 *
 * This function saves an SOA image to a PPM file.
 *
 * @param srcImage The image in SOA format.
 * @param metadata Metadata of the image.
 * @param maxLevel The maximum intensity level.
 * @param outputPath The path where the image will be saved.
 * @return int 0 if the image was saved correctly, 1 if there was an error opening the file or
 *             if the pixel format is not supported.
 */
int saveSOAtoPPM(const ImageSOA& srcImage, const PPMMetadata& metadata, const int maxLevel, const std::string& outputPath) {
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open() || outputPath.substr(outputPath.size() - 4) != ".ppm") {
        throw std::runtime_error("No se pudo abrir el archivo");
    }
    if (maxLevel < 0 || maxLevel > MAX_COLOR_VALUE_16) {
        throw std::runtime_error("Nivel máximo de intensidad no soportado");
    }
    // Check channel sizes
    if (std::holds_alternative<std::vector<uint8_t>>(srcImage.redChannel)) {
        auto redChannel = std::get<std::vector<uint8_t>>(srcImage.redChannel);
        auto greenChannel = std::get<std::vector<uint8_t>>(srcImage.greenChannel);
        auto blueChannel = std::get<std::vector<uint8_t>>(srcImage.blueChannel);
        // Verificar que los canales tengan el mismo tamaño
        if (redChannel.size() != greenChannel.size() || greenChannel.size() != blueChannel.size() || metadata.width * metadata.height != redChannel.size()) {
            throw std::runtime_error("Los tamaños de los canales no coinciden");
        }
        writePPMHeader(outFile, metadata, maxLevel);
        ColorChannels8 const channels8{.red=redChannel, .green=greenChannel, .blue=blueChannel};
        writePixelData8(outFile, channels8, maxLevel);
    } else if (std::holds_alternative<std::vector<uint16_t>>(srcImage.redChannel)) {
        auto redChannel = std::get<std::vector<uint16_t>>(srcImage.redChannel);
        auto greenChannel = std::get<std::vector<uint16_t>>(srcImage.greenChannel);
        auto blueChannel = std::get<std::vector<uint16_t>>(srcImage.blueChannel);
        // Verificar que los canales tengan el mismo tamaño
        if (redChannel.size() != greenChannel.size() || greenChannel.size() != blueChannel.size() || metadata.width * metadata.height != redChannel.size()) {
            throw std::runtime_error("Los tamaños de los canales no coinciden");
        }
        writePPMHeader(outFile, metadata, maxLevel);
        ColorChannels16 const channels16{.red=redChannel, .green=greenChannel, .blue=blueChannel};
        writePixelData16(outFile, channels16, maxLevel);
    } else {
        throw std::runtime_error("Formato de píxeles no soportado");
    }
    outFile.close();
    std::cout << "La imagen con el nuevo nivel máximo de intensidad se ha guardado en " << outputPath << '\n';
    return 0;
}

/**
 * @brief Prints the values of an SOA image.
 *
 * This function prints the values of the red, green, and blue channels of an SOA image.
 *
 * @param imagen The SOA image to print.
 * @param metadata Metadata of the image.
 */
void imprimirImagenSOA(const ImageSOA& imagen, const PPMMetadata& metadata) {
    // Obtener el número total de píxeles
    size_t const num_pixels = metadata.width * metadata.height;

    // Función lambda para imprimir los valores de un canal
    auto imprimirCanal = [num_pixels](const auto& canal, const std::string& nombre) {
        std::cout << nombre << " Channel:\n";
        for (size_t i = 0; i < num_pixels; ++i) {
            std::cout << static_cast<int>(canal[i]) << " "; // Usar static_cast para mostrar uint8_t correctamente
        }
        std::cout << '\n';
    };
    // Imprimir canal rojo
    if (std::holds_alternative<std::vector<uint8_t>>(imagen.redChannel)) {
        imprimirCanal(std::get<std::vector<uint8_t>>(imagen.redChannel), "Red");
    } else {
        imprimirCanal(std::get<std::vector<uint16_t>>(imagen.redChannel), "Red");
    }
    // Imprimir canal verde
    if (std::holds_alternative<std::vector<uint8_t>>(imagen.greenChannel)) {
        imprimirCanal(std::get<std::vector<uint8_t>>(imagen.greenChannel), "Green");
    } else {
        imprimirCanal(std::get<std::vector<uint16_t>>(imagen.greenChannel), "Green");
    }
    // Imprimir canal azul
    if (std::holds_alternative<std::vector<uint8_t>>(imagen.blueChannel)) {
        imprimirCanal(std::get<std::vector<uint8_t>>(imagen.blueChannel), "Blue");
    } else {
        imprimirCanal(std::get<std::vector<uint16_t>>(imagen.blueChannel), "Blue");
    }
}
