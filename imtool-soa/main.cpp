//
// Created by mapor on 07/10/2024.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include "common/progargs.hpp" // Este archivo contendría las funciones comunes
#include "imgsoa/imagesoa.hpp" // Librería específica de SOA para manipulación de imágenes


void printUsage() {
    std::cerr << "Usage: imtool <input_path> <output_path> <operation> [<additional_args>...]" << std::endl;
}

int main(int argc, char* argv[]) {
    // Verificar que el número de argumentos sea al menos tres
    if (argc < 4) {
        printUsage();
        std::cerr << "Error: Invalid number of arguments: " << argc - 1 << std::endl;
        return -1;
    }

    // Leer los argumentos de entrada
    std::string inputPath = argv[1];
    std::string outputPath = argv[2];
    std::string operation = argv[3];

    // Manejar cada operación
    if (operation == "info") {
        if (argc != 4) {
            std::cerr << "Error: Invalid extra arguments for info" << std::endl;
            return -1;
        }
        // Lógica para obtener metadatos de la imagen
        std::cout << "Executing 'info' operation on: " << inputPath << std::endl;

    } else if (operation == "maxlevel") {
        if (argc != 5) {
            std::cerr << "Error: Invalid number of extra arguments for maxlevel" << std::endl;
            return -1;
        }

        int newMaxLevel = std::atoi(argv[4]);
        if (newMaxLevel < 0 || newMaxLevel > 65535) {
            std::cerr << "Error: Invalid maxlevel: " << argv[4] << std::endl;
            return -1;
        }

        // Lógica para escalar el valor máximo de intensidad
        std::cout << "Executing 'maxlevel' operation with level: " << newMaxLevel << " on: " << inputPath << std::endl;

    } else if (operation == "resize") {
        if (argc != 6) {
            std::cerr << "Error: Invalid number of extra arguments for resize" << std::endl;
            return -1;
        }

        int newWidth = std::atoi(argv[4]);
        int newHeight = std::atoi(argv[5]);
        if (newWidth <= 0) {
            std::cerr << "Error: Invalid resize width: " << argv[4] << std::endl;
            return -1;
        }
        if (newHeight <= 0) {
            std::cerr << "Error: Invalid resize height: " << argv[5] << std::endl;
            return -1;
        }

        // Lógica para escalar la imagen a un nuevo tamaño
        std::cout << "Executing 'resize' operation with width: " << newWidth << " and height: " << newHeight << " on: " << inputPath << std::endl;

    } else if (operation == "cutfreq") {
        if (argc != 5) {
            std::cerr << "Error: Invalid number of extra arguments for cutfreq" << std::endl;
            return -1;
        }

        int numberOfColors = std::atoi(argv[4]);
        if (numberOfColors <= 0) {
            std::cerr << "Error: Invalid cutfreq: " << argv[4] << std::endl;
            return -1;
        }

        // Lógica para eliminar colores menos frecuentes
        std::cout << "Executing 'cutfreq' operation with number of colors: " << numberOfColors << " on: " << inputPath << std::endl;

    } else if (operation == "compress") {
        if (argc != 4) {
            std::cerr << "Error: Invalid extra arguments for compress" << std::endl;
            return -1;
        }

        // Lógica para comprimir la imagen
        std::cout << "Executing 'compress' operation on: " << inputPath << std::endl;

    } else {
        std::cerr << "Error: Invalid option: " << operation << std::endl;
        return -1;
    }

    // Aquí iría el código específico para procesar la imagen y escribir el archivo de salida
    // ...

    return 0;
}
