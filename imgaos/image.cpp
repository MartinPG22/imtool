//
// Created by mapor on 07/10/2024.
//
#include "./common/binaryio.hpp"  // Para readBinaryFile
#include "image.hpp"
#include <fstream>
#include <stdexcept>
/*
ImageAOS loadPPM_AOS(const std::string& filename) {
    // Leer el archivo binario completo en un vector de bytes
    std::vector<uint8_t> fileData = readBinaryFile(filename);

    size_t index = 0;  // Para recorrer el vector fileData

    std::string format;
    while (fileData[index] != '\n') {
        format += fileData[index++];
    }
    ++index;  // Saltar el '\n'

    if (format != "P6") {
        throw std::runtime_error("Formato PPM no soportado.");
    }

    // Leer ancho, alto y valor máximo de color
    size_t width = 0, height = 0;
    int max_value = 0;

    // Función auxiliar para leer un número desde fileData
    auto readNextInt = [&fileData, &index]() -> int {
        std::string num;
        while (fileData[index] == ' ' || fileData[index] == '\n') {
            ++index;  // Saltar espacios y saltos de línea
        }
        while (fileData[index] >= '0' && fileData[index] <= '9') {
            num += fileData[index++];
        }
        return std::stoi(num);
    };
    // Leer ancho, alto y max_value del encabezado
    width = readNextInt();
    height = readNextInt();
    max_value = readNextInt();

    // Asegurarse de que max_value esté en el rango esperado
    if (max_value > 255) {
        throw std::runtime_error("Máximo valor de color no soportado.");
    }

    // Ignorar el salto de línea que sigue al encabezado
    while (fileData[index] == '\n' || fileData[index] == ' ') {
        ++index;
    }

    // Crear la imagen AOS (Array of Structures)
    ImageAOS image(width, height);

    // Leer los píxeles a partir del vector de datos
    for (size_t i = 0; i < width * height; ++i) {
        uint8_t red = fileData[index++];
        uint8_t green = fileData[index++];
        uint8_t blue = fileData[index++];

        image.pixels[i] = { red, green, blue };
    }

    return image;
}
*/