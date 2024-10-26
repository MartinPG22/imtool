//
// Created by mapor on 26/10/2024.
//

#include "imagesoa.hpp"
#include "./common/binaryio.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <variant>
#include <string>

ImageSOA cargarImagenPPMSOA(const std::string& nombre_archivo, PPMMetadata& metadata) {
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

    ImageSOA imagen;
    size_t num_pixels = metadata.width * metadata.height;
    // Leer los píxeles en función del valor máximo
    if (metadata.max_value <= 255) {
        imagen.redChannel = std::vector<uint8_t>(num_pixels);
        imagen.greenChannel = std::vector<uint8_t>(num_pixels);
        imagen.blueChannel = std::vector<uint8_t>(num_pixels);

        auto& red = std::get<std::vector<uint8_t>>(imagen.redChannel);
        auto& green = std::get<std::vector<uint8_t>>(imagen.greenChannel);
        auto& blue = std::get<std::vector<uint8_t>>(imagen.blueChannel);

        for (size_t i = 0; i < num_pixels; i++) {
            archivo.read(reinterpret_cast<char*>(&red[i]), 1);
            archivo.read(reinterpret_cast<char*>(&green[i]), 1);
            archivo.read(reinterpret_cast<char*>(&blue[i]), 1);
        }
    } else {
        // Caso: 2 bytes por componente (RGB) en formato little-endian
        imagen.redChannel = std::vector<uint16_t>(num_pixels);
        imagen.greenChannel = std::vector<uint16_t>(num_pixels);
        imagen.blueChannel = std::vector<uint16_t>(num_pixels);

        auto& red = std::get<std::vector<uint16_t>>(imagen.redChannel);
        auto& green = std::get<std::vector<uint16_t>>(imagen.greenChannel);
        auto& blue = std::get<std::vector<uint16_t>>(imagen.blueChannel);

        for (size_t i = 0; i < num_pixels; ++i) {
            uint8_t r_low, r_high, g_low, g_high, b_low, b_high;

            archivo.read(reinterpret_cast<char*>(&r_low), 1);
            archivo.read(reinterpret_cast<char*>(&r_high), 1);
            archivo.read(reinterpret_cast<char*>(&g_low), 1);
            archivo.read(reinterpret_cast<char*>(&g_high), 1);
            archivo.read(reinterpret_cast<char*>(&b_low), 1);
            archivo.read(reinterpret_cast<char*>(&b_high), 1);

            // Convertir a little-endian (colocar el byte bajo en la posición menos significativa
            uint16_t r = static_cast<uint16_t>(r_low | (r_high << 8)); // Combinar bytes en little-endian
            uint16_t g = static_cast<uint16_t>(g_low | (g_high << 8)); // Combinar bytes en little-endian
            uint16_t b = static_cast<uint16_t>(b_low | (b_high << 8)); // Combinar bytes en little-endian

            red[i] = r;
            green[i] = g;
            blue[i] = b;
        }
    }
    return imagen;
}

// Función para imprimir la estructura ImageSOA
void imprimirImagenSOA(const ImageSOA& imagen, const PPMMetadata& metadata) {
    // Obtener el número total de píxeles
    size_t num_pixels = metadata.width * metadata.height;

    // Función lambda para imprimir los valores de un canal
    auto imprimirCanal = [num_pixels](const auto& canal, const std::string& nombre) {
        std::cout << nombre << " Channel:\n";
        for (size_t i = 0; i < num_pixels; ++i) {
            std::cout << static_cast<int>(canal[i]) << " "; // Usar static_cast para mostrar uint8_t correctamente
        }
        std::cout << std::endl;
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