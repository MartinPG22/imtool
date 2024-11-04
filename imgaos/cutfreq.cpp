//
// Created by mapor on 29/10/2024.
//

#include "cutfreq.hpp"


// Función para combinar los componentes RGB en un solo valor entero
template <typename T>
constexpr int combineRGB(const Color<T>& color) {
    return (color.r() << changeBits16) | (color.g() << changeBits) | color.b();
}

template <typename T>
constexpr Color<T> extractRGB(int rgb) {
    T blue = rgb & MAX_PIXEL_VALUE;                  // Últimos 8 bits son B
    T green = (rgb >> changeBits) & MAX_PIXEL_VALUE; // Siguientes 8 bits son G
    T red = (rgb >> changeBits16) & MAX_PIXEL_VALUE; // Primeros 8 bits son R

    return Color<T>(red, green, blue);
}

// Contar frecuencia de cada color en la imagen
void countColorFrequency(const ImageAOS& srcImage, std::unordered_map<int, int>& colorFrequency) {
    if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels)) {
        const auto& pixels8 = std::get<std::vector<Pixel8>>(srcImage.pixels);
        for (const auto& pixel : pixels8) {
            Color<int> const color(pixel.r, pixel.g, pixel.b);
            int const key = combineRGB(color); // Genera la clave única
            colorFrequency[key]++;
        }
    } else if (std::holds_alternative<std::vector<Pixel16>>(srcImage.pixels)) {
        const auto& pixels16 = std::get<std::vector<Pixel16>>(srcImage.pixels);
        for (const auto& pixel : pixels16) {
            Color<int> const color(pixel.r, pixel.g, pixel.b);
            int const key = combineRGB(color); // Genera la clave única
            colorFrequency[key]++;
        }
    }
}

// Ordenar los colores por frecuencia y luego por componentes de color
void sortColorsByFrequency(std::unordered_map<int, int>& colorFrequency, std::vector<std::tuple<int, Color<int>>>& colorData) {
    for (const auto& [key, frequency] : colorFrequency) {
        Color<int> const color = extractRGB<int>(key);
        colorData.emplace_back(frequency, color);
    }

    std::sort(colorData.begin(), colorData.end(), [](const auto& inicio, const auto& final) {
        if (std::get<0>(inicio) != std::get<0>(final)) { return std::get<0>(inicio) < std::get<0>(final);
}
        if (std::get<1>(inicio).b() != std::get<1>(final).b()) { return std::get<1>(inicio).b() > std::get<1>(final).b();
}
        if (std::get<1>(inicio).g() != std::get<1>(final).g()) { return std::get<1>(inicio).g() > std::get<1>(final).g();
}
        return std::get<1>(inicio).r() > std::get<1>(final).r();
    });
}


// Dividir colores en los que se mantienen y los que se eliminan
ColorSplit splitColors(const std::vector<std::tuple<int, Color<int>>>& colorData, int nColores) {
    ColorSplit splitResult;
    splitResult.colorsToRemove = {colorData.begin(), colorData.begin() + nColores};
    splitResult.colorsToKeep = {colorData.begin() + nColores, colorData.end()};
    return splitResult;
}

// Encontrar el color más cercano de los colores que se mantienen
Color<int> findClosestColor(const Color<int>& colorRem, const std::vector<std::tuple<int, Color<int>>>& colorsToKeep) {
    double minDistance = std::numeric_limits<double>::max();
    Color<int> closestColor(0, 0, 0);

    for (const auto& [freqKeep, colorKeep] : colorsToKeep) {
        double const distance = std::sqrt(
            std::pow(colorRem.r() - colorKeep.r(), 2) +
            std::pow(colorRem.g() - colorKeep.g(), 2) +
            std::pow(colorRem.b() - colorKeep.b(), 2));

        if (distance < minDistance) {
            minDistance = distance;
            closestColor = colorKeep;
        }
    }
    return closestColor;
}


// Crear un mapa de reemplazo para colores a eliminar
void createReplacementMap(const ColorGroups& colorGroups,
                          std::unordered_map<int, Color<int>>& replacementMap) {
    for (const auto& [freqRem, colorRem] : colorGroups.colorsToRemove) {
        Color<int> const closestColor = findClosestColor(colorRem, colorGroups.colorsToKeep);
        replacementMap[combineRGB(colorRem)] = closestColor;
    }
}

// Aplicar reemplazo de colores en la imagen
void applyColorReplacement(ImageAOS& srcImage, const std::unordered_map<int, Color<int>>& replacementMap) {
    if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels)) {
        auto& pixels8 = std::get<std::vector<Pixel8>>(srcImage.pixels);
        for (auto& pixel : pixels8) {
            Color<int> const color(pixel.r, pixel.g, pixel.b);
            int const key = combineRGB(color);
            if (replacementMap.find(key) != replacementMap.end()) {
                Color<int> const newColor = replacementMap.at(key);
                pixel.r = static_cast<uint8_t>(newColor.r());
                pixel.g = static_cast<uint8_t>(newColor.g());
                pixel.b = static_cast<uint8_t>(newColor.b());
            }
        }
    } else if (std::holds_alternative<std::vector<Pixel16>>(srcImage.pixels)) {
        auto& pixels16 = std::get<std::vector<Pixel16>>(srcImage.pixels);
        for (auto& pixel : pixels16) {
            Color<int> const color(pixel.r, pixel.g, pixel.b);
            int const key = combineRGB(color);
            if (replacementMap.find(key) != replacementMap.end()) {
                Color<int> const newColor = replacementMap.at(key);
                pixel.r = static_cast<uint16_t>(newColor.r());
                pixel.g = static_cast<uint16_t>(newColor.g());
                pixel.b = static_cast<uint16_t>(newColor.b());
            }
        }
    }
}

// Función principal cutfreq que usa las funciones auxiliares
void cutfreq(ImageAOS& srcImage, const PPMMetadata& metadata, int nColores, const std::string& outputPath) {
    std::unordered_map<int, int> colorFrequency;
    countColorFrequency(srcImage, colorFrequency);

    std::vector<std::tuple<int, Color<int>>> colorData;
    sortColorsByFrequency(colorFrequency, colorData);

    // Llamada a splitColors
    ColorSplit const colorSplit = splitColors(colorData, nColores);
    ColorGroups const colorGroups{.colorsToRemove=colorSplit.colorsToRemove, .colorsToKeep=colorSplit.colorsToKeep};

    std::unordered_map<int, Color<int>> replacementMap;
    createReplacementMap(colorGroups, replacementMap);

    applyColorReplacement(srcImage, replacementMap);

    saveAOStoPPM(srcImage, metadata, (metadata.max_value), outputPath);
}

