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
#include <array>


// Función auxiliar para leer canales con componentes de 8 bits
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

// Función auxiliar para leer canales con componentes de 16 bits
void leerCanales16Bits(std::ifstream& archivo, ImageSOA& imagen, const size_t num_pixels) {
    auto& red = std::get<std::vector<uint16_t>>(imagen.redChannel);
    auto& green = std::get<std::vector<uint16_t>>(imagen.greenChannel);
    auto& blue = std::get<std::vector<uint16_t>>(imagen.blueChannel);

    std::array<char, 1> buffer{};
    for (size_t i = 0; i < num_pixels; ++i) {
        archivo.read(buffer.data(), 1);
        auto r_low = static_cast<uint8_t>(buffer[0]);

        archivo.read(buffer.data(), 1);
        auto r_high = static_cast<uint8_t>(buffer[0]);

        archivo.read(buffer.data(), 1);
        auto g_low = static_cast<uint8_t>(buffer[0]);

        archivo.read(buffer.data(), 1);
        auto g_high = static_cast<uint8_t>(buffer[0]);

        archivo.read(buffer.data(), 1);
        auto b_low = static_cast<uint8_t>(buffer[0]);

        archivo.read(buffer.data(), 1);
        auto b_high = static_cast<uint8_t>(buffer[0]);

        red[i] = static_cast<uint16_t>(r_low | (r_high << BYTE_SIZE));
        green[i] = static_cast<uint16_t>(g_low | (g_high << BYTE_SIZE));
        blue[i] = static_cast<uint16_t>(b_low | (b_high << BYTE_SIZE));
    }
}

// Función para cargar una imagen PPM en formato SOA
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

// Función para imprimir la estructura ImageSOA
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