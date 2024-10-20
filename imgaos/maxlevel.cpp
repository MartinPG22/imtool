//
// Created by asanch on 10/19/24.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"

struct Pixel {
    int red, green, blue; // Cambiar a uint16_t para soportar hasta 65535
};

int maxlevel() {
    std::ifstream file("/home/asanch/arcos/PracticaArcos/imagesPPM/deer-small.ppm", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo" << std::endl;
        return 1;
    }

    std::string format;
    file >> format;

    if (format != "P6") {
        std::cerr << "Formato PPM no soportado: " << format << std::endl;
        return 1;
    }

    int width, height, maxColorValue;
    file >> width >> height >> maxColorValue;
    file.ignore();  // Ignorar el salto de línea después del encabezado
    std::cout << "Ancho: " << width << ", Alto: " << height << ", Valor máximo de color: " << maxColorValue << std::endl;

    using size_type = std::vector<Pixel>::size_type;
    std::vector<std::vector<Pixel>> image(static_cast<size_type>(height), std::vector<Pixel>(static_cast<size_type>(width)));

    // Leer los valores de los píxeles
    if (maxColorValue <= 255) {
        // Formato estándar P6 (cada componente de color es 1 byte)
        for (std::size_t i = 0; i < static_cast<std::size_t>(height); ++i) {
            for (std::size_t j = 0; j < static_cast<std::size_t>(width); ++j) {
                Pixel pixel{};
                file.read(reinterpret_cast<char*>(&pixel.red), 1);
                file.read(reinterpret_cast<char*>(&pixel.green), 1);
                file.read(reinterpret_cast<char*>(&pixel.blue), 1);
                image[i][j] = pixel;
            }
        }
    } else if (maxColorValue <= 65535) {
        // Formato P6 con valor máximo de color mayor a 255 (cada componente de color es 2 bytes)
        for (std::size_t i = 0; i < static_cast<std::size_t>(height); ++i) {
            for (std::size_t j = 0; j < static_cast<std::size_t>(width); ++j) {
                Pixel pixel{};
                // Leer los dos bytes para cada componente de color en little-endian
                uint8_t bytes[2];

                // Rojo
                file.read(reinterpret_cast<char*>(&bytes), 2);
                pixel.red = (bytes[1] << 8) | bytes[0];

                // Verde
                file.read(reinterpret_cast<char*>(&bytes), 2);
                pixel.green = (bytes[1] << 8) | bytes[0];

                // Azul
                file.read(reinterpret_cast<char*>(&bytes), 2);
                pixel.blue = (bytes[1] << 8) | bytes[0];

                image[i][j] = pixel;
            }
        }
    } else {
        std::cerr << "El valor máximo de color no es válido: " << maxColorValue << std::endl;
        return 1;
    }

    file.close();

    // Mostrar la imagen en la consola (solo los primeros píxeles)
    for (std::size_t i = 0; i < std::min(static_cast<std::size_t>(10), image.size()); ++i) {
        for (std::size_t j = 0; j < std::min(static_cast<std::size_t>(10), image[i].size()); ++j) {
            std::cout << "(" << image[i][j].red << ", "
                      << image[i][j].green << ", "
                      << image[i][j].blue << ") ";
        }
        std::cout << std::endl;
    }

    // Cambiar al nuevo nivel máximo de intensidad
    for (auto & i : image) {
        for (auto &[red, green, blue] : i) {
            constexpr int newMaxLevel = 128;
            red = red * newMaxLevel / maxColorValue;
            green = green * newMaxLevel / maxColorValue;
            blue = blue * newMaxLevel / maxColorValue;
        }
    }

    // Guardar el vector de píxeles en un nuevo archivo PPM
    std::ofstream outFile("/home/asanch/arcos/PracticaArcos/imagesPPM/output.ppm", std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo de salida" << std::endl;
        return 1;
    }
    // Escribir el encabezado
    outFile << "P6\n" << width << " " << height << "\n" << 128 << "\n";
    // Escribir los valores de los píxeles en el formato binario
    for (const auto& row : image) {
        for (const auto&[red, green, blue] : row) {
            outFile.write(reinterpret_cast<const char*>(&red), 1);
            outFile.write(reinterpret_cast<const char*>(&green), 1);
            outFile.write(reinterpret_cast<const char*>(&blue), 1);
        }
    }
    outFile.close();
    std::cout << "La imagen con el nuevo nivel máximo de intensidad se ha guardado en output.ppm." << std::endl;

    return 0;
}
