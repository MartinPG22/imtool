//
// Created by mapor on 29/10/2024.
//

#include "cutfreq.hpp"


// Función para guardar la imagen en formato PPM
void saveToPPM(const ImageAOS& srcImage, const PPMMetadata& metadata, const std::string& outputPath) {
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo de salida" << '\n';
        return; // Salir si no se puede abrir el archivo
    }// Puede que este mal la intensidad del encabezado
    outFile << "P6\n" << metadata.width << " " << metadata.height << "\n" << metadata.max_value << "\n"; // Encabezado para Pixel8

    // Escribir los datos de los píxeles
    if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels)) {
        const auto& pixels8 = std::get<std::vector<Pixel8>>(srcImage.pixels);
        for (const auto& pixel : pixels8) {
            outFile.put(static_cast<char>(pixel.r));
            outFile.put(static_cast<char>(pixel.g));
            outFile.put(static_cast<char>(pixel.b));
        }
    } else if (std::holds_alternative<std::vector<Pixel16>>(srcImage.pixels)) {
        const auto& pixels16 = std::get<std::vector<Pixel16>>(srcImage.pixels);
        for (const auto& pixel : pixels16) {
                outFile.put(static_cast<char>(pixel.r >> changeBits)); // Escribir el byte alto
                outFile.put(static_cast<char>(pixel.r & MAX_PIXEL_VALUE)); // Escribir el byte bajo
                outFile.put(static_cast<char>(pixel.g >> changeBits)); // Escribir el byte alto
                outFile.put(static_cast<char>(pixel.g & MAX_PIXEL_VALUE)); // Escribir el byte bajo
                outFile.put(static_cast<char>(pixel.b >> changeBits)); // Escribir el byte alto
                outFile.put(static_cast<char>(pixel.b & MAX_PIXEL_VALUE)); // Escribir el byte bajo
            }
    }

    outFile.close();
    std::cout << "Imagen guardada en: " << outputPath << '\n';
}

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

/*
void cutfreq(ImageAOS& srcImage, const PPMMetadata& metadata, int nColores, const std::string& outputPath) {
    // Mapa para almacenar la frecuencia de cada color (RGB combinado en una clave)
    std::unordered_map<int, int> colorFrequency;

    // Procesamiento de píxeles dependiendo del tipo de píxel en ImageAOS
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

    // Vector para almacenar los colores con sus frecuencias
    std::vector<std::tuple<int, Color<int>>> colorData; // {frecuencia, Color}

    // Convertimos cada entrada en colorFrequency a una tupla y la añadimos al vector colorData
    for (const auto& [key, frequency] : colorFrequency) {
        Color<int> const color = extractRGB<int>(key);
        colorData.emplace_back(frequency, color);
    }

    // Ordenar por frecuencia ascendente, luego por b, g, r descendente
    std::sort(colorData.begin(), colorData.end(), [](const auto& inicio, const auto& final) {
        if (std::get<0>(inicio) != std::get<0>(final)) { return std::get<0>(inicio) < std::get<0>(final); // Frecuencia ascendente
}
        if (std::get<1>(inicio).b() != std::get<1>(final).b()) { return std::get<1>(inicio).b() > std::get<1>(final).b(); // b descendente
}
        if (std::get<1>(inicio).g() != std::get<1>(final).g()) { return std::get<1>(inicio).g() > std::get<1>(final).g(); // g descendente
}
        return std::get<1>(inicio).r() > std::get<1>(final).r(); // r descendente
    });

    // Dividimos los colores en los que se van a eliminar y los que se mantienen
    std::vector<std::tuple<int, Color<int>>> const colorsToRemove(colorData.begin(), colorData.begin() + nColores);
    std::vector<std::tuple<int, Color<int>>> const colorsToKeep(colorData.begin() + nColores, colorData.end());

    // Mapa de sustitución para reemplazar colores eliminados por el color más cercano
    std::unordered_map<int, Color<int>> replacementMap;

    // Calcula el color más cercano para cada color a eliminar
    for (const auto& [freqRem, colorRem] : colorsToRemove) {
        double minDistance = std::numeric_limits<double>::max();
        Color<int> closestColor(0,0,0);

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

        replacementMap[combineRGB(colorRem)] = closestColor; // Mapeo de color original al color más cercano
    }

    // Reemplazar colores en la imagen original
    if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels)) {
        auto& pixels8 = std::get<std::vector<Pixel8>>(srcImage.pixels);
        for (auto& pixel : pixels8) {
            Color<int> const color(pixel.r, pixel.g, pixel.b);
            int const key = combineRGB(color);
            if (replacementMap.find(key) != replacementMap.end()) {
                Color<int> const newColor = replacementMap[key];
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
                Color<int> const newColor = replacementMap[key];
                pixel.r = static_cast<uint16_t>(newColor.r());
                pixel.g = static_cast<uint16_t>(newColor.g());
                pixel.b = static_cast<uint16_t>(newColor.b());
            }
        }
    }

    std::cout << "Colores reemplazados en la imagen.\n";
    std::cout << "(DEPURACION) LLamando a todos los argumentos para evitar errores de clangtidy" << metadata.width<< outputPath<<'\n';
    saveToPPM(srcImage, metadata, outputPath); // Guardar la imagen modificada
}*/

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

    saveToPPM(srcImage, metadata, outputPath);
}

