//
// Created by asanch on 10/19/24.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#include "imageaos.hpp"


int maxlevel(const ImageAOS& srcImage,
    const PPMMetadata& metadata, const int newMaxLevel, const std::string& outputPath) {
    /*
    // TODO: Pasar esto al metodo de cargarImagenPPM e ImageAOS.hpp
    std::string format;
    file >> format;
    if (format != "P6") {
        std::cerr << "Formato PPM no soportado: " << format << '\n';
        return 1;
    }
    int width = 0;
    int height = 0;
    int maxColorValue = 0;
    file >> width >> height >> maxColorValue;
    file.ignore();  // Ignorar el salto de línea después del encabezado
    std::cout << "Ancho: " << width << ", Alto: " << height << ", Valor máximo de color: " << maxColorValue << '\n';
    ///////////////////////////////////////////////////////////////////*/

    // Mostrar en la consola los primeros píxeles de srcImage
    const auto& pixels8 = std::get<std::vector<Pixel8>>(srcImage.pixels);
    for (size_t y_cord = 0; y_cord < metadata.height; ++y_cord) {
        for (size_t x_cord = 0; x_cord < metadata.width; ++x_cord) {
            size_t const index = (y_cord * metadata.width) + x_cord;
            if (index >= pixels8.size()) { break; }; // Evitar overflow si hay menos de 100 píxeles
            const auto&[r, g, b] = pixels8[index];
            std::cout << "(" << static_cast<int>(r) << ", "
                      << static_cast<int>(g) << ", "
                      << static_cast<int>(b) << ") ";
        }
        std::cout << '\n';
    }

    // Cambiar al nuevo nivel máximo de intensidad, hace una copia de la imagen
    std::vector<Pixel8> out_pixels8;
    size_t index = 0;
    for (const auto&[r, g, b] : pixels8) {
        out_pixels8[index].r = static_cast<uint8_t>(r * newMaxLevel / metadata.max_value);
        out_pixels8[index].g = static_cast<uint8_t>(g * newMaxLevel / metadata.max_value);
        out_pixels8[index].b = static_cast<uint8_t>(b * newMaxLevel / metadata.max_value);
        ++index;
    }

    // Guardar el vector de píxeles en un nuevo archivo PPM
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo de salida" << '\n';
        return 1;
    }
    // Escribir el encabezado
    outFile << "P6\n" << metadata.width << " " << metadata.height << "\n" << newMaxLevel << "\n";
    // Escribir los valores de los píxeles en el formato binario
    for (const auto&[r, g, b] : out_pixels8) {
        outFile.put(static_cast<char>(r)); // Escribir el valor rojo
        outFile.put(static_cast<char>(g)); // Escribir el valor verde
        outFile.put(static_cast<char>(b)); // Escribir el valor azul
    }
    outFile.close();
    std::cout << "La imagen con el nuevo nivel máximo de intensidad se ha guardado en output.ppm." << '\n';

    return 0;
}
