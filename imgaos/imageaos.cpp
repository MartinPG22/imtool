//
// Created by mapor on 19/10/2024.
//

#include "imageaos.hpp"


/**
 * @brief Loads 8-bit pixels from a file to an AOS data structure.
 *
 * This function reads 8-bit pixels from a file and stores them in an AOS image.
 *
 * @param archivo The file stream to read the pixels from.
 * @param num_pixels The number of pixels to read.
 * @return ImageAOS The image in AOS format.
 */
ImageAOS cargarPixels8(std::ifstream& archivo, const size_t num_pixels) {
    ImageAOS imagen;
    std::vector<Pixel8> pixels8(num_pixels);

    std::array<char, buffer_size_8> buffer{};
    size_t count = 0;
    for (auto&[r, g, b] : pixels8) {
        if (!archivo.read(buffer.data(), buffer_size_8)) {
            std::cerr << "Error: Falló la lectura de píxeles." << '\n';
            return {};
        }
        r = static_cast<uint8_t>(buffer[0]);
        g = static_cast<uint8_t>(buffer[1]);
        b = static_cast<uint8_t>(buffer[2]);
        count++;
    }

    std::cerr << "Se leyeron " << count << " píxeles." << '\n';
    imagen.pixels = std::move(pixels8);
    return imagen;
}

/**
 * @brief Loads 16-bit pixels from a file to an AOS data structure.
 *
 * This function reads 8-bit pixels from a file and stores them in an AOS image.
 *
 * @param archivo The file stream to read the pixels from.
 * @param num_pixels The number of pixels to read.
 * @return ImageAOS The image in AOS format.
 */
ImageAOS cargarPixels16(std::ifstream& archivo, const size_t num_pixels) {
    ImageAOS imagen;
    std::vector<Pixel16> pixels16(num_pixels);

    std::array<char, buffer_size_16> buffer{};
    for (auto&[r, g, b] : pixels16) {
        if (!archivo.read(buffer.data(), buffer_size_16)) {
            std::cerr << "Error: Falló la lectura de píxeles." << '\n';
            return {};
        }
        // Usar el tipo correcto durante la combinación de bits
        uint16_t const red1 = static_cast<uint8_t>(buffer[0]);
        uint16_t const red2 = static_cast<uint8_t>(buffer[1]);
        uint16_t const green1 = static_cast<uint8_t>(buffer[2]);
        uint16_t const green2 = static_cast<uint8_t>(buffer[3]);
        uint16_t const blue1 = static_cast<uint8_t>(buffer[4]);
        uint16_t const blue2 = static_cast<uint8_t>(buffer[5]);
        // Realizar las combinaciones usando suma
        r = static_cast<uint16_t>(red1 | (red2 << changeBits));
        g = static_cast<uint16_t>(green1 | (green2 << changeBits));
        b = static_cast<uint16_t>(blue1 | (blue2 << changeBits));
    }
    imagen.pixels = std::move(pixels16);
    return imagen;
}

/**
 * @brief Loads an image in PPM format to an AOS data structure.
 *
 * This function loads an image in PPM (P6) format to an image in AOS (Array of Structures) format.
 * The function can handle images with 8-bit or 16-bit pixels.
 *
 * @param nombre_archivo The path to the image file.
 * @param metadata Metadata of the image.
 * @return ImageAOS The image in AOS format or an empty image if the file cannot be opened.
 */
ImageAOS cargarImagenPPMAOS(const std::string& nombre_archivo, PPMMetadata& metadata) {
    std::ifstream archivo(nombre_archivo, std::ios::binary);
    if (!archivo) {
        std::cerr << "Error: No se puede abrir el archivo." << '\n';
        return {};
    }
    // Leer el número mágico
    std::string magicNumber;
    archivo >> magicNumber;
    if (magicNumber != "P6") {
        std::cerr << "Error: Formato no soportado." << '\n';
        return {};
    }
    // Leer dimensiones y valor máximo
    archivo >> metadata.width >> metadata.height >> metadata.max_value;
    archivo.ignore(); // Ignorar el salto de línea después del encabezado
    // Crear la imagen en formato AOS
    ImageAOS const imagen;
    size_t const num_pixels = metadata.width * metadata.height;
    if (metadata.max_value <= MAX_8) {
        return cargarPixels8(archivo, num_pixels); // Llamar a función auxiliar
    }
    return cargarPixels16(archivo, num_pixels); // Llamar a función auxiliar
}

/**
 * @brief Saves an AOS image to a PPM file.
 *
 * This function saves an image in AOS (Array of Structures) format to a file in PPM (P6) format.
 * The function can handle images with 8-bit or 16-bit pixels.
 *
 * @param srcImage The image in AOS format.
 * @param metadata Metadata of the image.
 * @param maxLevel The new maximum level of the pixels.
 * @param outputPath The path where the image with the new maximum level will be saved.
 * @return int 0 if the image was saved successfully, 1 if there was an error opening the file or
 *             if the pixel format is not supported.
 * @throws std::runtime_error If the output file cannot be opened or if the pixel format is not supported.
 */
int saveAOStoPPM(const ImageAOS& srcImage, const PPMMetadata& metadata, const int maxLevel, const std::string& outputPath) {
    std::variant<std::vector<Pixel8>, std::vector<Pixel16>> const pixels = srcImage.pixels;
    // Abrir el archivo de salida
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo de salida" << '\n';
        return 1;
    }
    // Escribir el encabezado
    outFile << "P6\n" << metadata.width << " " << metadata.height << "\n" << maxLevel << "\n";
    // Escribir los valores de los píxeles en el formato binario
    if (std::holds_alternative<std::vector<Pixel8>>(pixels)) {
        const auto& pixelData = std::get<std::vector<Pixel8>>(pixels);
        for (const auto& [r, g, b] : pixelData) {
            outFile.put(static_cast<char>(r));
            outFile.put(static_cast<char>(g));
            outFile.put(static_cast<char>(b));
        }
    } else if (std::holds_alternative<std::vector<Pixel16>>(pixels)) {
        const auto& pixelData = std::get<std::vector<Pixel16>>(pixels);
        for (const auto& [r, g, b] : pixelData) {
            outFile.put(static_cast<char>(r));
            outFile.put(static_cast<char>(g));
            outFile.put(static_cast<char>(b));
        }
    } else {
        throw std::runtime_error("Unsupported pixel format");
    }
    outFile.close();
    std::cout << "La imagen con el nuevo nivel máximo de intensidad se ha guardado en " << outputPath << '\n';
    return 0;
}
