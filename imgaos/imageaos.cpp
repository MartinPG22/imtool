//
// Created by mapor on 19/10/2024.
//

#include "imageaos.hpp"
#include "./common/binaryio.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <variant>
#include <string>

ImageAOS cargarImagenPPM(const std::string& nombre_archivo, PPMMetadata& metadata) {
    std::ifstream archivo(nombre_archivo, std::ios::binary);
    if (!archivo) {
        std::cerr << "Error: No se puede abrir el archivo." << std::endl;
        return {};
    }

    // Leer el tipo de magia
    std::string magicNumber;
    archivo >> magicNumber;
    if (magicNumber != "P6") {
        std::cerr << "Error: Formato no soportado." << std::endl;
        return {};
    }

    // Leer dimensiones y valor máximo
    archivo >> metadata.width >> metadata.height >> metadata.max_value;
    archivo.ignore(); // Ignorar el salto de línea después del encabezado

    ImageAOS imagen;
    size_t num_pixels = metadata.width * metadata.height;

    // Leer los píxeles en función del valor máximo
    if (metadata.max_value <= 255) {
        std::vector<Pixel8> pixels8(num_pixels);
        for (auto& px : pixels8) {
            archivo.read(reinterpret_cast<char*>(&px.r), 1);
            archivo.read(reinterpret_cast<char*>(&px.g), 1);
            archivo.read(reinterpret_cast<char*>(&px.b), 1);
        }
        imagen.pixels = std::move(pixels8);
    } else {
        // Caso: 2 bytes por componente (RGB) en formato little-endian
        std::vector<Pixel16> pixels16(num_pixels);
        for (auto& px : pixels16) {
            // Leer los dos bytes en little-endian y combinarlos correctamente
            uint8_t byte1, byte2;

            archivo.read(reinterpret_cast<char*>(&byte1), 1); // Primer byte del rojo
            archivo.read(reinterpret_cast<char*>(&byte2), 1); // Segundo byte del rojo
            px.r = static_cast<uint16_t>(byte1 | (byte2 << 8)); // Combinar bytes en little-endian

            archivo.read(reinterpret_cast<char*>(&byte1), 1); // Primer byte del verde
            archivo.read(reinterpret_cast<char*>(&byte2), 1); // Segundo byte del verde
            px.g = static_cast<uint16_t>(byte1 | (byte2 << 8)); // Combinar bytes en little-endian

            archivo.read(reinterpret_cast<char*>(&byte1), 1); // Primer byte del azul
            archivo.read(reinterpret_cast<char*>(&byte2), 1); // Segundo byte del azul
            px.b = static_cast<uint16_t>(byte1 | (byte2 << 8)); // Combinar bytes en little-endian
        }
        imagen.pixels = std::move(pixels16);
    }

    return imagen;
}


void imprimirPixeles(const ImageAOS& imagen, PPMMetadata& metadata) {
    // Determinamos si los píxeles son de tipo Pixel8 o Pixel16
    if (std::holds_alternative<std::vector<Pixel8>>(imagen.pixels)) {
        const auto& pixels = std::get<std::vector<Pixel8>>(imagen.pixels);
        std::cout << "Imagen de " << metadata.width << "x" << metadata.height << " con píxeles de 8 bits.\n";

        for (size_t i = 0; i < metadata.height; ++i) {
            for (size_t j = 0; j < metadata.width; ++j) {
                const auto& pixel = pixels[i * metadata.width + j];
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
                const auto& pixel = pixels[i * metadata.width + j];
                std::cout << "(" << pixel.r << ", "
                          << pixel.g << ", "
                          << pixel.b << ") ";
            }
            std::cout << "\n";
        }
    }
}//