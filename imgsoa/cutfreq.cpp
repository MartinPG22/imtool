//
// Created by mapor on 31/10/2024.
//

#include "imagesoa.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
#include "cutfreq.hpp"




// Contar frecuencia de cada color en la imagen SOA
void countColorFrequency(const ImageSOA& srcImage,
                         std::unordered_map<int, int>& colorFrequency) {
    if (std::holds_alternative<std::vector<uint8_t>>(srcImage.redChannel)) {
        const auto& red = std::get<std::vector<uint8_t>>(srcImage.redChannel);
        const auto& green = std::get<std::vector<uint8_t>>(srcImage.greenChannel);
        const auto& blue = std::get<std::vector<uint8_t>>(srcImage.blueChannel);

        for (size_t i = 0; i < red.size(); ++i) {
            int const key = combineRGB(red[i], green[i], blue[i]);
            colorFrequency[key]++;
        }
    } else {
        const auto& red = std::get<std::vector<uint16_t>>(srcImage.redChannel);
        const auto& green = std::get<std::vector<uint16_t>>(srcImage.greenChannel);
        const auto& blue = std::get<std::vector<uint16_t>>(srcImage.blueChannel);

        for (size_t i = 0; i < red.size(); ++i) {
            int const key = combineRGB(red[i], green[i], blue[i]);
            colorFrequency[key]++;
        }
    }
}

// Reemplaza los colores en la imagen SOA según el mapa de reemplazo
void applyColorReplacement(
        ImageSOA& srcImage,
        const std::unordered_map<int, std::tuple<int, int, int>>& replacementMap) {
    if (std::holds_alternative<std::vector<uint8_t>>(srcImage.redChannel)) {
        auto& red = std::get<std::vector<uint8_t>>(srcImage.redChannel);
        auto& green = std::get<std::vector<uint8_t>>(srcImage.greenChannel);
        auto& blue = std::get<std::vector<uint8_t>>(srcImage.blueChannel);

        for (size_t i = 0; i < red.size(); ++i) {
            int const key = combineRGB(red[i], green[i], blue[i]);
            if (replacementMap.find(key) != replacementMap.end()) {
                auto [newR, newG, newB] = replacementMap.at(key);
                red[i] = static_cast<uint8_t>(newR);
                green[i] = static_cast<uint8_t>(newG);
                blue[i] = static_cast<uint8_t>(newB);
            }
        }
    } else {
        auto& red = std::get<std::vector<uint16_t>>(srcImage.redChannel);
        auto& green = std::get<std::vector<uint16_t>>(srcImage.greenChannel);
        auto& blue = std::get<std::vector<uint16_t>>(srcImage.blueChannel);

        for (size_t i = 0; i < red.size(); ++i) {
            int const key = combineRGB(red[i], green[i], blue[i]);
            if (replacementMap.find(key) != replacementMap.end()) {
                auto [newR, newG, newB] = replacementMap.at(key);
                red[i] = static_cast<uint16_t>(newR);
                green[i] = static_cast<uint16_t>(newG);
                blue[i] = static_cast<uint16_t>(newB);
            }
        }
    }
}
// Función que convierte el mapa de frecuencia en un vector de tuplas y lo ordena
std::vector<std::tuple<int, int, int, int>> getColorData(const std::unordered_map<int, int>& colorFrequency) {
    std::vector<std::tuple<int, int, int, int>> colorData; // {frecuencia, r, g, b}
    for (const auto& [key, frequency] : colorFrequency) {
        int const red = (key >> 16) & MAX_COLOR_VALUE;
        int const green = (key >> BYTE_SIZE) & MAX_COLOR_VALUE;
        int const blue = key & MAX_COLOR_VALUE;
        colorData.emplace_back(frequency, red, green, blue);
    }
    std::sort(colorData.begin(), colorData.end(),
              [](const auto& tupla1, const auto& tupla2) { return std::get<0>(tupla1) < std::get<0>(tupla2); });
    return colorData;
}

// Función para buscar el color más cercano en la lista de colores
std::tuple<int, int, int> findClosestReplacement(const std::tuple<int, int, int>& targetColor,
                                                 const std::vector<std::tuple<int, int, int, int>>& colorData,
                                                 size_t startIdx) {
    int rRem = 0;
    int gRem = 0;
    int bRem = 0;
    std::tie(rRem, gRem, bRem) = targetColor; // Extrae los tres valores de targetColor

    double minDistance = std::numeric_limits<double>::max();
    std::tuple<int, int, int> closestColor;

    for (size_t j = startIdx; j < colorData.size(); ++j) {
        int ignored = 0;
        int rKeep = 0;
        int gKeep = 0;
        int bKeep = 0;
        std::tie(ignored, rKeep, gKeep, bKeep) = colorData[j]; // Extrae los cuatro valores en colorData[j]

        double const distance = std::sqrt(std::pow(rRem - rKeep, 2) + std::pow(gRem - gKeep, 2) +
                                          std::pow(bRem - bKeep, 2));
        if (distance < minDistance) {
            minDistance = distance;
            closestColor = std::make_tuple(rKeep, gKeep, bKeep);
        }
    }
    return closestColor;
}

// Función para crear el mapa de reemplazo de colores
std::unordered_map<int, std::tuple<int, int, int>> createReplacementMap(
    const std::vector<std::tuple<int, int, int, int>>& colorData, int nColores) {

    auto nColoresSize = static_cast<size_t>(nColores);
    std::unordered_map<int, std::tuple<int, int, int>> replacementMap;

    for (size_t i = 0; i < nColoresSize && i < colorData.size(); ++i) {
        int freqRem = 0;
        int rRem = 0;
        int gRem = 0;
        int bRem = 0;
        std::tie(freqRem, rRem, gRem, bRem) = colorData[i]; // Extrae los valores de color

        int const remKey = combineRGB(rRem, gRem, bRem);

        // Buscar el color más cercano para reemplazar
        std::tuple<int, int, int> const closestColor = findClosestReplacement(
            std::make_tuple(rRem, gRem, bRem), colorData, nColoresSize); // Solo pasar RGB

        replacementMap[remKey] = closestColor;
    }
    return replacementMap;
}

// Función para aplicar el reemplazo de colores y guardar la imagen resultante
void applyReplacementsAndSave(ImageSOA& srcImage,
                              const std::unordered_map<int, std::tuple<int, int, int>>& replacementMap,
                              const PPMMetadata& metadata, const std::string& outputPath) {
    applyColorReplacement(srcImage, replacementMap);
    saveSOAtoPPM(srcImage, metadata, metadata.max_value, outputPath);
}

// Función principal cutfreq para procesar la imagen SOA
void cutfreqSOA(ImageSOA& srcImage, const PPMMetadata& metadata, int nColores,
                const std::string& outputPath) {
    // Contar la frecuencia de colores en la imagen
    std::unordered_map<int, int> colorFrequency;
    countColorFrequency(srcImage, colorFrequency);

    // Obtener y ordenar datos de colores
    auto colorData = getColorData(colorFrequency);

    // Crear mapa de reemplazo de colores
    auto replacementMap = createReplacementMap(colorData, nColores);

    // Aplicar reemplazos y guardar la imagen
    applyReplacementsAndSave(srcImage, replacementMap, metadata, outputPath);
}

