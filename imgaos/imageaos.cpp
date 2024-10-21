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

// Función para cargar la imagen PPM y transformarla a AOS
ImageAOS cargarImagenPPM(const std::string& nombre_archivo, PPMMetadata& metadata) {
    std::ifstream archivo(nombre_archivo, std::ios::binary);
    if (!archivo) {
        std::cerr << "Error: No se puede abrir el archivo." << std::endl;
        return {};
    }

    ImageAOS imagen;

    size_t num_pixels = metadata.width * metadata.height;

    // Leer los píxeles en función del valor máximo
    if (metadata.max_value <= 255) {
        // Caso: 1 byte por componente (RGB) -> max_value <= 255
        std::vector<Pixel8> pixels8(num_pixels);
        for (auto& px : pixels8) {
            archivo.read(reinterpret_cast<char*>(&px.r), 1);
            archivo.read(reinterpret_cast<char*>(&px.g), 1);
            archivo.read(reinterpret_cast<char*>(&px.b), 1);
        }
        imagen.pixels = std::move(pixels8);
    } else {
        // Caso: 2 bytes por componente (RGB) -> max_value > 255
        std::vector<Pixel16> pixels16(num_pixels);
        for (auto& px : pixels16) {
            archivo.read(reinterpret_cast<char*>(&px.r), 2);
            archivo.read(reinterpret_cast<char*>(&px.g), 2);
            archivo.read(reinterpret_cast<char*>(&px.b), 2);
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