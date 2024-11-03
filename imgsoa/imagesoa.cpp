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
     */
    void leerCanales8Bits(std::ifstream& archivo, ImageSOA& imagen, const size_t num_pixels) {
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
        }
    }
    /** @brief Reads the channels with 16-bit components.
     *
     * This function reads the red, green, and blue channels with 16-bit components from a file.
     *
     * @param archivo The file stream to read the channels from.
     * @param imagen The SOA image to store the channels.
     * @param num_pixels The number of pixels to read.
     */
    void leerCanales16Bits(std::ifstream& archivo, ImageSOA& imagen, const size_t num_pixels) {
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
 */
ImageSOA cargarImagenPPMSOA(const std::string& nombre_archivo, PPMMetadata& metadata) {
    std::ifstream archivo(nombre_archivo, std::ios::binary);
    if (!archivo) {
        std::cerr << "Error: No se puede abrir el archivo." << '\n';
        return {};
    }
    // Leer el tipo de magia
    std::string magicNumber;
    archivo >> magicNumber;
    if (magicNumber != "P6") {
        std::cerr << "Error: Formato no soportado." << '\n';
        return {};
    }
    // Leer dimensiones y valor máximo
    archivo >> metadata.width >> metadata.height >> metadata.max_value;
    archivo.ignore(); // Ignorar el salto de línea después del encabezado
    // Crear la estructura ImageSOA
    ImageSOA imagen;
    size_t const num_pixels = metadata.width * metadata.height;
    // Leer los píxeles en función del valor máximo
    if (metadata.max_value <= METATADATA_MAX_VALUE) {
        imagen.redChannel = std::vector<uint8_t>(num_pixels);
        imagen.greenChannel = std::vector<uint8_t>(num_pixels);
        imagen.blueChannel = std::vector<uint8_t>(num_pixels);
        leerCanales8Bits(archivo, imagen, num_pixels);
    } else {
        // Caso: 2 bytes por componente (RGB) en formato little-endian
        imagen.redChannel = std::vector<uint16_t>(num_pixels);
        imagen.greenChannel = std::vector<uint16_t>(num_pixels);
        imagen.blueChannel = std::vector<uint16_t>(num_pixels);
        leerCanales16Bits(archivo, imagen, num_pixels);
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
    if (!outFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo de salida" << '\n';
        return 1;
    }
    // Check channel sizes
    if (std::holds_alternative<std::vector<uint8_t>>(srcImage.redChannel) &&
        std::holds_alternative<std::vector<uint8_t>>(srcImage.greenChannel) &&
        std::holds_alternative<std::vector<uint8_t>>(srcImage.blueChannel)) {
        auto redChannel = std::get<std::vector<uint8_t>>(srcImage.redChannel);
        auto greenChannel = std::get<std::vector<uint8_t>>(srcImage.greenChannel);
        auto blueChannel = std::get<std::vector<uint8_t>>(srcImage.blueChannel);
        // Verificar que los canales tengan el mismo tamaño
        if (redChannel.size() != greenChannel.size() || greenChannel.size() != blueChannel.size()) {
            std::cerr << "Channel sizes do not match!" << '\n';
            return 1;
        }
        // Escribir el encabezado
        outFile << "P6\n" << metadata.width << " " << metadata.height << "\n" << maxLevel << "\n";
        // Escribir los valores de los píxeles en el formato binario
        for (size_t i = 0; i < redChannel.size(); ++i) {
            outFile.put(static_cast<char>(redChannel[i]));
            outFile.put(static_cast<char>(greenChannel[i]));
            outFile.put(static_cast<char>(blueChannel[i]));
        }
    } else if (std::holds_alternative<std::vector<uint16_t>>(srcImage.redChannel) &&
           std::holds_alternative<std::vector<uint16_t>>(srcImage.greenChannel) &&
           std::holds_alternative<std::vector<uint16_t>>(srcImage.blueChannel)) {
        auto redChannel = std::get<std::vector<uint16_t>>(srcImage.redChannel);
        auto greenChannel = std::get<std::vector<uint16_t>>(srcImage.greenChannel);
        auto blueChannel = std::get<std::vector<uint16_t>>(srcImage.blueChannel);
        // Verificar que los canales tengan el mismo tamaño
        if (redChannel.size() != greenChannel.size() || greenChannel.size() != blueChannel.size()) {
            std::cerr << "Channel sizes do not match!" << '\n';
            return 1;
        }
        // Escribir el encabezado
        outFile << "P6\n" << metadata.width << " " << metadata.height << "\n" << maxLevel << "\n";
        // Escribir los valores de los píxeles en el formato binario
        for (size_t i = 0; i < redChannel.size(); ++i) {
            outFile.put(static_cast<char>(redChannel[i] * maxLevel / MAX_16));
            outFile.put(static_cast<char>(greenChannel[i] * maxLevel / MAX_16));
            outFile.put(static_cast<char>(blueChannel[i] * maxLevel / MAX_16));
        }
    } else {
        std::cerr << "Unsupported pixel format!" << '\n';
        return 1;
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