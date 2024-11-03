//
// Created by asanch on 11/2/24.
//

#include "maxlevel.hpp"

struct ImageData {
    std::vector<uint8_t> red;
    std::vector<uint8_t> green;
    std::vector<uint8_t> blue;
    PPMMetadata metadata;
    int maxLevel;
};

namespace {
    int saveImage(const ImageData& imageData, const std::string& outputPath) {
        std::ofstream outFile(outputPath, std::ios::binary);
        if (!outFile.is_open()) {
            std::cerr << "No se pudo abrir el archivo de salida" << '\n';
            return 1;
        }

        // Escribir el encabezado
        outFile << "P6\n" << imageData.metadata.width << " " << imageData.metadata.height << "\n" << imageData.maxLevel << "\n";

        // Escribir los valores de los píxeles en el formato binario
        for (size_t i = 0; i < imageData.red.size(); ++i) {
            outFile.put(static_cast<char>(imageData.red[i]));
            outFile.put(static_cast<char>(imageData.green[i]));
            outFile.put(static_cast<char>(imageData.blue[i]));
        }

        outFile.close();
        std::cout << "La imagen con el nuevo nivel máximo de intensidad se ha guardado en " << outputPath << '\n';
        return 0;
    }
}

int maxlevelSOA(const ImageSOA& srcImage, const PPMMetadata& metadata, const int newMaxLevel, const std::string& outputPath) {
    // Verificar que los canales tengan el mismo tamaño
    const auto& red = std::get<std::vector<uint8_t>>(srcImage.redChannel);
    const auto& green = std::get<std::vector<uint8_t>>(srcImage.greenChannel);
    const auto& blue = std::get<std::vector<uint8_t>>(srcImage.blueChannel);
    if (red.size() != green.size() || red.size() != blue.size()) {
        std::cerr << "Error: Los canales de color no tienen el mismo tamaño\n";
        return 1;
    }
    // Convertir los valores de los píxeles
    std::vector<uint8_t> outRed(red.size());
    std::vector<uint8_t> outGreen(green.size());
    std::vector<uint8_t> outBlue(blue.size());
    for (size_t i = 0; i < red.size(); ++i) {
        outRed[i] = static_cast<uint8_t>(red[i] * newMaxLevel / metadata.max_value);
        outGreen[i] = static_cast<uint8_t>(green[i] * newMaxLevel / metadata.max_value);
        outBlue[i] = static_cast<uint8_t>(blue[i] * newMaxLevel / metadata.max_value);
    }
    // Guardar la imagen
    const ImageData imageData = { .red=outRed, .green=outGreen, .blue=outBlue, .metadata=metadata, .maxLevel=newMaxLevel };
    return saveImage(imageData, outputPath);
}
