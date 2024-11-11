#include "cutfreq.hpp"

// Función para combinar colores RGB (8 bits por canal)
template <typename T>
constexpr u_int64_t combineRGB(const Color<T>& color) {
    return (static_cast<uint64_t>(color.r()) << RED_SHIFT) |
           (static_cast<uint64_t>(color.g()) << GREEN_SHIFT) |
           static_cast<uint64_t>(color.b());
}

// Función para combinar colores RGB48 (16 bits por canal)
template <typename T>
constexpr u_int64_t combineRGB48(const Color<T>& color) {
    return (static_cast<uint64_t>(color.r() & COLOR_MASK_16BIT) << RED_SHIFT_48) |
           (static_cast<uint64_t>(color.g() & COLOR_MASK_16BIT) << GREEN_SHIFT_48) |
           (static_cast<uint64_t>(color.b() & COLOR_MASK_16BIT));
}

// Función para extraer colores RGB8
template <typename T>
constexpr Color<T> extractRGB8(const uint64_t rgb) {
    // Extraer componentes para uint8_t (asumiendo que rgb es un entero de 24 bits)
    T blue = rgb & MAX_COLOR_VALUE_16UINT;                          // Últimos 8 bits son B
    T green = (rgb >> MIN_PIXEL_VALUE) & MAX_COLOR_VALUE_16UINT;                  // Siguientes 8 bits son G
    T red = (rgb >> MIN_PIXEL_VALUE_16) & MAX_COLOR_VALUE_16UINT;                   // Primeros 8 bits son R

    return Color<T>(red, green, blue);
}

// Función para extraer colores RGB48
template <typename T>
constexpr Color<T> extractRGB48(const uint64_t rgb) {
    // Extraer componentes para uint16_t (asumiendo que rgb es un entero de 64 bits)
    const uint16_t red = (rgb >> RED_SHIFT_48) & COLOR_MASK_16BIT;   // Primeros 16 bits son R
    const uint16_t green = (rgb >> GREEN_SHIFT_48) & COLOR_MASK_16BIT; // Siguientes 16 bits son G
     const uint16_t blue = rgb & COLOR_MASK_16BIT;          // Últimos 16 bits son B

    return Color<T>(red, green, blue);
}

// Contar frecuencia de cada color en la imagen
void countColorFrequency(const ImageAOS& srcImage, std::unordered_map<uint64_t, int>& colorFrequency) {
    if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels)) {
        const auto& pixels8 = std::get<std::vector<Pixel8>>(srcImage.pixels);
        for (const auto& pixel : pixels8) {
            Color<uint8_t> const color(pixel.r, pixel.g, pixel.b);
            uint64_t const key = combineRGB(color); // Genera la clave única
            colorFrequency[key]++;
        }
    } else if (std::holds_alternative<std::vector<Pixel16>>(srcImage.pixels)) {
        const auto& pixels16 = std::get<std::vector<Pixel16>>(srcImage.pixels);
        for (const auto& pixel : pixels16) {
            Color<uint16_t> const color(pixel.r, pixel.g, pixel.b);
            uint64_t const key = combineRGB48(color); // Genera la clave única
            colorFrequency[key]++;
        }
    } else {
        std::cerr << "Formato de imagen no compatible.\n";
    }
}

// Ordenar los colores por frecuencia y luego por componentes de color
void sortColorsByFrequency(std::unordered_map<uint64_t, int>& colorFrequency, std::vector<std::tuple<int, Color<int>>>& colorData, int maxLevel) {
    if (maxLevel <= METATADATA_MAX_VALUE) {
        for (const auto& [key, frequency] : colorFrequency) {
            Color<int> const color = extractRGB8<int>(key);
            colorData.emplace_back(frequency, color);
        }
    } else {
        for (const auto& [key, frequency] : colorFrequency) {
            Color<int> const color = extractRGB48<int>(key);
            colorData.emplace_back(frequency, color);
        }
    }

    std::sort(colorData.begin(), colorData.end(), [](const auto& inicio, const auto& final) {
        if (std::get<0>(inicio) != std::get<0>(final)) { return std::get<0>(inicio) < std::get<0>(final); }
        if (std::get<1>(inicio).b() != std::get<1>(final).b()) { return std::get<1>(inicio).b() > std::get<1>(final).b(); }
        if (std::get<1>(inicio).g() != std::get<1>(final).g()) { return std::get<1>(inicio).g() > std::get<1>(final).g(); }
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
                          std::unordered_map<uint64_t, Color<int>>& replacementMap) {
    for (const auto& [freqRem, colorRem] : colorGroups.colorsToRemove) {
        // Supongamos que tienes una manera de determinar si el color es de tipo Pixel8 o Pixel16
        uint64_t key = 0;

        // Determina la forma de combinación según el rango de valores del color
        if (colorRem.r() <= METATADATA_MAX_VALUE && colorRem.g() <= METATADATA_MAX_VALUE && colorRem.b() <= METATADATA_MAX_VALUE) {
            // Usar combineRGB para Pixel8
            key = combineRGB(colorRem);
        } else {
            // Usar combineRGB48 para Pixel16
            key = combineRGB48(colorRem);
        }

        Color<int> const closestColor = findClosestColor(colorRem, colorGroups.colorsToKeep);
        replacementMap[key] = closestColor;
    }
}


// Aplicar reemplazo de colores en la imagen
void applyColorReplacement(ImageAOS& srcImage, const std::unordered_map<uint64_t, Color<int>>& replacementMap) {
    if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels)) {
        auto& pixels8 = std::get<std::vector<Pixel8>>(srcImage.pixels);
        for (auto& pixel : pixels8) {
            Color<int> const color(pixel.r, pixel.g, pixel.b);
            uint64_t const key = combineRGB(color);
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
            uint64_t const key = combineRGB48(color);
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
    std::unordered_map<uint64_t, int> colorFrequency;
    countColorFrequency(srcImage, colorFrequency);

    std::vector<std::tuple<int, Color<int>>> colorData;
    sortColorsByFrequency(colorFrequency, colorData, metadata.max_value);

    // Llamada a splitColors
    ColorSplit const colorSplit = splitColors(colorData, nColores);
    ColorGroups const colorGroups{.colorsToRemove=colorSplit.colorsToRemove, .colorsToKeep=colorSplit.colorsToKeep};

    std::unordered_map<uint64_t, Color<int>> replacementMap;
    createReplacementMap(colorGroups, replacementMap);

    applyColorReplacement(srcImage, replacementMap);

    saveAOStoPPM(srcImage, metadata, (metadata.max_value), outputPath);
}