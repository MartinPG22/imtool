//
// Created by asanch on 11/2/24.
//

#include "maxlevel.hpp"

namespace {
    // Funcion auxiliar para verificar si la imagen tiene píxeles de 8 bits
    bool is8Bit(const ImageSOA& srcImage) {
        return std::holds_alternative<std::vector<uint8_t>>(srcImage.redChannel);
    }
    // Funcion auxiliar para verificar si la imagen tiene píxeles de 16 bits
    bool is16Bit(const ImageSOA& srcImage) {
        return std::holds_alternative<std::vector<uint16_t>>(srcImage.redChannel);
    }
    // Funcion auxiliar para verificar los canales de color tienen el mismo tamaño
    template <typename T>
    std::tuple<std::vector<T>, std::vector<T>, std::vector<T>> verifyChannels(const ImageSOA& srcImage) {
        const auto& red = std::get<std::vector<T>>(srcImage.redChannel);
        const auto& green = std::get<std::vector<T>>(srcImage.greenChannel);
        const auto& blue = std::get<std::vector<T>>(srcImage.blueChannel);
        if (red.size() != green.size() || red.size() != blue.size() || red.empty()) {
            throw std::runtime_error("Error: Los canales de color no tienen el mismo tamaño");
        }
        return { red, green, blue };
    }
    // Funcion auxiliar para verificar el nivel máximo de intensidad
    void verifyMaxLevel(const int newMaxLevel, const PPMMetadata& metadata, const ImageSOA& srcImage) {
        if (is8Bit(srcImage)) {
            if ((newMaxLevel <= 0) || (newMaxLevel > MAX_COLOR_VALUE)) {
                throw std::runtime_error("Error: Nivel máximo de intensidad inválido: " + std::to_string(newMaxLevel));
            }
            if ((metadata.max_value <= 0) || (metadata.max_value > MAX_COLOR_VALUE)) {
                throw std::runtime_error("Error: Nivel máximo de intensidad inválido: " + std::to_string(metadata.max_value));
            }
        } else if (is16Bit(srcImage)) {
            if ((newMaxLevel <= MAX_COLOR_VALUE) || (newMaxLevel > MAX_COLOR_VALUE_16)) {
                throw std::runtime_error("Error: Nivel máximo de intensidad inválido: " + std::to_string(newMaxLevel));
            }
            if ((metadata.max_value <= MAX_COLOR_VALUE) || (metadata.max_value > MAX_COLOR_VALUE_16)) {
                throw std::runtime_error("Error: Nivel máximo de intensidad inválido: " + std::to_string(metadata.max_value));
            }
        } else {
            throw std::runtime_error("Error: Formato de píxeles no compatible\n");
        }
    }
    // Funcion auxiliar que ejecuta el maxlevel para un tipo de píxeles
    template <typename T>
    ImageSOA maxlevelSOAtype(const ImageSOA& srcImage, const PPMMetadata& metadata, const int newMaxLevel, const std::string& outputPath) {
        auto channels = verifyChannels<T>(srcImage);
        auto& [red, green, blue] = channels;
        // Verificar que newMaxLevel y metada.max_level sea válido
        verifyMaxLevel(newMaxLevel, metadata, srcImage);
        // Convertir los valores de los píxeles
        std::vector<T> outRed(red.size());
        std::vector<T> outGreen(green.size());
        std::vector<T> outBlue(blue.size());
        if (metadata.width * metadata.height != outRed.size()) {
            throw std::runtime_error("Invalid number of pixels");
        }
        std::cout << "Cambiando el nivel máximo de intensidad de la imagen a " << newMaxLevel << '\n';
        for (size_t i = 0; i < red.size(); ++i) {
            outRed[i] = static_cast<T>(red[i] * newMaxLevel / metadata.max_value);
            outGreen[i] = static_cast<T>(green[i] * newMaxLevel / metadata.max_value);
            outBlue[i] = static_cast<T>(blue[i] * newMaxLevel / metadata.max_value);
        }
        // Guardar la imagen
        const ImageSOA max_level_SOA = { .redChannel=outRed, .greenChannel=outGreen, .blueChannel=outBlue };
        const PPMMetadata new_metadata = { .width=metadata.width, .height=metadata.height, .max_value=newMaxLevel };
        saveSOAtoPPM(max_level_SOA, new_metadata, newMaxLevel, outputPath);
        return max_level_SOA;
    }
}

/**
 * @brief Cambia el nivel máximo de intensidad de una imagen en formato SOA.
 *
 * Esta función cambia el nivel máximo de intensidad de una imagen en formato SOA.
 *
 * @param srcImage La imagen en formato SOA.
 * @param metadata Metadatos de la imagen.
 * @param newMaxLevel El nuevo nivel máximo de intensidad.
 * @param outputPath La ruta donde se guardará la imagen con el nuevo nivel máximo.
 * @return La imagen con el nuevo nivel máximo de intensidad.
 * @throws std::runtime_error Si no se puede abrir el archivo de salida o si el formato de píxeles no es compatible.
 */
ImageSOA maxlevelSOA(const ImageSOA& srcImage, const PPMMetadata& metadata, const int newMaxLevel, const std::string& outputPath) {
    // Verificar que tipo de píxeles tiene la imagen
    if (is8Bit(srcImage)) {
        // Ejecutar la función para píxeles de 8 bits
        return maxlevelSOAtype<uint8_t>(srcImage, metadata, newMaxLevel, outputPath);
    } if (is16Bit(srcImage)) {
        // Ejecutar la función para píxeles de 16 bits
        return maxlevelSOAtype<uint16_t>(srcImage, metadata, newMaxLevel, outputPath);
    }
    throw std::runtime_error("Error: Formato de píxeles no compatible\n");
}
