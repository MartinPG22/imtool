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
    template <typename PixelType>
    void leerCanales8Bits(std::ifstream& archivo, ImageSOA<PixelType>& imagen, const size_t num_pixels) {
        auto& red = imagen.redChannel;
        auto& green = imagen.greenChannel;
        auto& blue = imagen.blueChannel;

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
    template <typename PixelType>
    void leerCanales16Bits(std::ifstream& archivo, ImageSOA<PixelType>& imagen, const size_t num_pixels) {
        auto& red = imagen.redChannel;
        auto& green = imagen.greenChannel;
        auto& blue = imagen.blueChannel;

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

            // Combine high and low bytes into a 16-bit value
            auto r_value = static_cast<uint16_t>(r_low | (r_high << BYTE_SIZE));
            auto g_value = static_cast<uint16_t>(g_low | (g_high << BYTE_SIZE));
            auto b_value = static_cast<uint16_t>(b_low | (b_high << BYTE_SIZE));

            // Assign to the vector, with potential narrowing if PixelType is uint8_t
            red[i] = static_cast<PixelType>(r_value);
            green[i] = static_cast<PixelType>(g_value);
            blue[i] = static_cast<PixelType>(b_value);
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
template <typename PixelType>
ImageSOA<PixelType> cargarImagenPPMSOA(const std::string& nombre_archivo, PPMMetadata& metadata) {
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
    ImageSOA<PixelType> imagen;
    size_t const num_pixels = metadata.width * metadata.height;
    imagen.redChannel = std::vector<PixelType>(num_pixels);
    imagen.greenChannel = std::vector<PixelType>(num_pixels);
    imagen.blueChannel = std::vector<PixelType>(num_pixels);
    // Leer los píxeles en función del valor máximo
    if (metadata.max_value <= METATADATA_MAX_VALUE) {
        leerCanales8Bits(archivo, imagen, num_pixels);
    } else {
        // Caso: 2 bytes por componente (RGB) en formato little-endian
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
template <typename PixelType>
int saveSOAtoPPM(const ImageSOA<PixelType>& srcImage, const PPMMetadata& metadata, const int maxLevel, const std::string& outputPath) {
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo de salida" << '\n';
        return 1;
    }
    auto redChannel = srcImage.redChannel;
    auto greenChannel = srcImage.greenChannel;
    auto blueChannel = srcImage.blueChannel;
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
template <typename PixelType>
void imprimirImagenSOA(const ImageSOA<PixelType>& imagen, const PPMMetadata& metadata) {
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
    imprimirCanal(std::vector<PixelType>(imagen.redChannel), "Red");
    // Imprimir canal verde
    imprimirCanal(std::vector<PixelType>(imagen.greenChannel), "Green");
    // Imprimir canal azul
    imprimirCanal(std::vector<PixelType>(imagen.blueChannel), "Blue");
}

// Explicit instantiation
template ImageSOA<uint8_t> cargarImagenPPMSOA(const std::string&, PPMMetadata&);
template ImageSOA<uint16_t> cargarImagenPPMSOA(const std::string&, PPMMetadata&);
template int saveSOAtoPPM(const ImageSOA<uint8_t>&, const PPMMetadata&, const int, const std::string&);
template int saveSOAtoPPM(const ImageSOA<uint16_t>&, const PPMMetadata&, const int, const std::string&);
template void imprimirImagenSOA(const ImageSOA<uint8_t>&, const PPMMetadata&);
template void imprimirImagenSOA(const ImageSOA<uint16_t>&, const PPMMetadata&);
