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

constexpr int MAX_COLOR_VALUE = 0xFF;

// Combina los valores RGB en una única clave entera
constexpr int combineRGB(int r, int g, int b) {
    return (r << 16) | (g << BYTE_SIZE) | b;
}

// Contar frecuencia de cada color en la imagen SOA
void countColorFrequency(const ImageSOA& srcImage,
                         std::unordered_map<int, int>& colorFrequency) {
    if (std::holds_alternative<std::vector<uint8_t>>(srcImage.redChannel)) {
        const auto& red = std::get<std::vector<uint8_t>>(srcImage.redChannel);
        const auto& green = std::get<std::vector<uint8_t>>(srcImage.greenChannel);
        const auto& blue = std::get<std::vector<uint8_t>>(srcImage.blueChannel);

        for (size_t i = 0; i < red.size(); ++i) {
            int key = combineRGB(red[i], green[i], blue[i]);
            colorFrequency[key]++;
        }
    } else {
        const auto& red = std::get<std::vector<uint16_t>>(srcImage.redChannel);
        const auto& green = std::get<std::vector<uint16_t>>(srcImage.greenChannel);
        const auto& blue = std::get<std::vector<uint16_t>>(srcImage.blueChannel);

        for (size_t i = 0; i < red.size(); ++i) {
            int key = combineRGB(red[i], green[i], blue[i]);
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
            int key = combineRGB(red[i], green[i], blue[i]);
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
            int key = combineRGB(red[i], green[i], blue[i]);
            if (replacementMap.find(key) != replacementMap.end()) {
                auto [newR, newG, newB] = replacementMap.at(key);
                red[i] = static_cast<uint16_t>(newR);
                green[i] = static_cast<uint16_t>(newG);
                blue[i] = static_cast<uint16_t>(newB);
            }
        }
    }
}

// Función principal cutfreq para procesar la imagen SOA
void cutfreqSOA(ImageSOA& srcImage, const PPMMetadata& metadata, int nColores,
             const std::string& outputPath) {
    std::unordered_map<int, int> colorFrequency;
    countColorFrequency(srcImage, colorFrequency);

    // Convertir mapa de frecuencias a vector y ordenar
    std::vector<std::tuple<int, int, int, int>> colorData; // {frecuencia, r, g, b}
    for (const auto& [key, frequency] : colorFrequency) {
        int r = (key >> 16) & MAX_COLOR_VALUE;
        int g = (key >> BYTE_SIZE) & MAX_COLOR_VALUE;
        int b = key & MAX_COLOR_VALUE;
        colorData.emplace_back(frequency, r, g, b);
    }
    std::sort(colorData.begin(), colorData.end(),
              [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });

    // Convertir nColores a size_t para evitar conflictos de signo
    size_t nColoresSize = static_cast<size_t>(nColores);

    // Dividir colores
    std::unordered_map<int, std::tuple<int, int, int>> replacementMap;
    for (size_t i = 0; i < nColoresSize && i < colorData.size(); ++i) {
        int freqRem, rRem, gRem, bRem;
        std::tie(freqRem, rRem, gRem, bRem) = colorData[i];
        int remKey = combineRGB(rRem, gRem, bRem);

        // Buscar color más cercano para reemplazar
        double minDistance = std::numeric_limits<double>::max();
        std::tuple<int, int, int> closestColor;
        for (size_t j = nColoresSize; j < colorData.size(); ++j) {
            int _, rKeep, gKeep, bKeep;
            std::tie(_, rKeep, gKeep, bKeep) = colorData[j];
            double distance = std::sqrt(std::pow(rRem - rKeep, 2) + std::pow(gRem - gKeep, 2) +
                                        std::pow(bRem - bKeep, 2));
            if (distance < minDistance) {
                minDistance = distance;
                closestColor = std::make_tuple(rKeep, gKeep, bKeep);
            }
        }
        replacementMap[remKey] = closestColor;
    }

    applyColorReplacement(srcImage, replacementMap);
    saveSOAtoPPM(srcImage, metadata, metadata.max_value, outputPath);
}
