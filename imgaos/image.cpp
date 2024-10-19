//
// Created by mapor on 07/10/2024.
//
#include "./common/binaryio.cpp"  // Para readBinaryFile
#include "image.hpp"
#include <fstream>
#include <stdexcept>

#include "image.hpp"
/*
// Función para obtener información del archivo PPM en formato AOS
ImageAOS infoPPM_AOS(const std::string& filename) {
    // Leer metadatos
    PPMMetadata metadata = readPPMMetadata(filename);

    // Crear la imagen AOS
    ImageAOS image(metadata.width, metadata.height);

    // Leer los píxeles a partir del archivo
    std::vector<uint8_t> fileData = readBinaryFile(filename);
    size_t index = 0;

    // Ignorar el encabezado (ya lo hemos leído)
    while (fileData[index] != '\n') {
        ++index;
    }
    ++index;  // Saltar el '\n' después del formato
    // Leer ancho y alto, ya hemos leído el valor máximo

    // Ignorar líneas vacías o espacios después de los metadatos
    while (fileData[index] == '\n' || fileData[index] == ' ') {
        ++index;
    }

    // Leer los píxeles
    for (size_t i = 0; i < metadata.width * metadata.height; ++i) {
        uint8_t red = fileData[index++];
        uint8_t green = fileData[index++];
        uint8_t blue = fileData[index++];

        image.pixels[i] = { red, green, blue };
    }

    return image;
}
*/
