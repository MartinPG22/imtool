//
// Created by mapor on 19/10/2024.
//

#include "imageaos.hpp"


// Función auxiliar para cargar píxeles de 8 bits
ImageAOS cargarPixels8(std::ifstream& archivo, size_t num_pixels) {
    ImageAOS imagen;
    std::vector<Pixel8> pixels8(num_pixels);

    std::array<char, buffer_size_8> buffer{};
    for (auto& pix : pixels8) {
        if (!archivo.read(buffer.data(), buffer_size_8)) {
            std::cerr << "Error: Falló la lectura de píxeles." << '\n';
            return {};
        }
        pix.r = static_cast<uint8_t>(buffer[0]);
        pix.g = static_cast<uint8_t>(buffer[1]);
        pix.b = static_cast<uint8_t>(buffer[2]);
    }
    imagen.pixels = std::move(pixels8);
    return imagen;
}

// Función auxiliar para cargar píxeles de 16 bits
ImageAOS cargarPixels16(std::ifstream& archivo, size_t num_pixels) {
    ImageAOS imagen;
    std::vector<Pixel16> pixels16(num_pixels);

    std::array<char, buffer_size_16> buffer{};
    for (auto& pix : pixels16) {
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
        pix.r = static_cast<uint16_t>(red1 + (red2 << changeBits));
        pix.g = static_cast<uint16_t>(green1 + (green2 << changeBits));
        pix.b = static_cast<uint16_t>(blue1 + (blue2 << changeBits));
    }
    imagen.pixels = std::move(pixels16);
    return imagen;
}

ImageAOS cargarImagenPPM(const std::string& nombre_archivo, PPMMetadata& metadata) {
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

    ImageAOS const imagen;
    size_t const num_pixels = metadata.width * metadata.height;

    if (metadata.max_value <= MAX_8) {
        return cargarPixels8(archivo, num_pixels); // Llamar a función auxiliar
    }         return cargarPixels16(archivo, num_pixels); // Llamar a función auxiliar

}



/*
void imprimirPixeles(const ImageAOS& imagen, PPMMetadata& metadata) {
    // Determinamos si los píxeles son de tipo Pixel8 o Pixel16
    if (std::holds_alternative<std::vector<Pixel8>>(imagen.pixels)) {
        const auto& pixels = std::get<std::vector<Pixel8>>(imagen.pixels);
        std::cout << "Imagen de " << metadata.width << "x" << metadata.height << " con píxeles de 8 bits.\n";

        for (size_t i = 0; i < metadata.height; ++i) {
            for (size_t j = 0; j < metadata.width; ++j) {
                const auto& pixel = pixels[(i * metadata.width) + j];
                std::cout << "(" << static_cast<int>(pixel.r) << ", "
                          << static_cast<int>(pixel.g) << ", "
                          << static_cast<int>(pixel.b) << ") ";
            }
            std::cout << "\n";
        }
    } else if (std::holds_alternative<std::vector<Pixel16>>(imagen.pixels)) {
        const auto& pixels = std::get<std::vector<Pixel16>>(imagen.pixels);
        std::cout << "Imagen de " << metadata.width << "x" << metadata.height << " con píxeles de 16 bits.\n";

        for (size_t i = 0; i < metadata.height; ++i) {
            for (size_t j = 0; j < metadata.width; ++j) {
                const auto& pixel = pixels[(i * metadata.width) + j];
                std::cout << "(" << pixel.r << ", "
                          << pixel.g << ", "
                          << pixel.b << ") ";
            }
            std::cout << "\n";
        }
    }
}*/