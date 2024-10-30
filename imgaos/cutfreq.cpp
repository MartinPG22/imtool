//
// Created by mapor on 29/10/2024.
//

#include "cutfreq.hpp"

/*void cutfreq(const ImageAOS& srcImage, const PPMMetadata& metadata, int nColores, const std::string& outputPath) {
    // Mapa para almacenar la frecuencia de cada color (r, g, b)
    std::unordered_map<std::string, int> colorFrequency;

    // Lambda para crear una clave única a partir de (r, g, b)
    auto generateKey = [](auto red, auto green, auto blue) {
        return std::to_string(red) + "," + std::to_string(green) + "," + std::to_string(blue);
    };

    // Procesamiento de píxeles dependiendo del tipo de píxel en ImageAOS
    if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels)) {
        const auto& pixels8 = std::get<std::vector<Pixel8>>(srcImage.pixels);
        for (const auto& pixel : pixels8) {
            std::string const key = generateKey(pixel.r, pixel.g, pixel.b);
            colorFrequency[key]++;
            std::cout << "(DEPURACION) Pixel8 - RGB(" << int(pixel.r) << ", " << int(pixel.g) << ", " << int(pixel.b) << ")\n";
        }
    } else if (std::holds_alternative<std::vector<Pixel16>>(srcImage.pixels)) {
        const auto& pixels16 = std::get<std::vector<Pixel16>>(srcImage.pixels);
        for (const auto& pixel : pixels16) {
            std::string const key = generateKey(pixel.r, pixel.g, pixel.b);
            colorFrequency[key]++;
            std::cout << "(DEPURACION) Pixel16 - RGB(" << pixel.r << ", " << pixel.g << ", " << pixel.b << ")\n";
        }
    }
    // Aqui ya estan los colores por frecuencia.
    // Vector para almacenar los colores con sus frecuencias
    std::vector<std::tuple<int, int, int, int>> colorData; // {frecuencia, r, g, b}
    // Convertimos cada entrada en colorFrequency a una tupla y la añadimos al vector colorData
    for (const auto& [key, frequency] : colorFrequency) {
        std::istringstream sss(key);
        int red = 0;
        int green = 0;
        int blue = 0;
        char comma = 0;
        sss >> red >> comma >> green >> comma >> blue;
        colorData.emplace_back(frequency, red, green, blue);
    }

    // Ordenamos el vector: primero por frecuencia ascendente, luego por b, g y r descendente
    std::sort(colorData.begin(), colorData.end(), [](const auto& inicio, const auto& final) {
        if (std::get<0>(inicio) != std::get<0>(final)) { return std::get<0>(inicio) < std::get<0>(final);  // Frecuencia ascendente
}
        if (std::get<3>(inicio) != std::get<3>(final)) { return std::get<3>(inicio) > std::get<3>(final);  // b descendente
}
        if (std::get<2>(inicio) != std::get<2>(final)) { return std::get<2>(inicio) > std::get<2>(final);  // g descendente
}
        return std::get<1>(inicio) > std::get<1>(final);                                        // r descendente
    });

    // Imprimimos los nColores menos frecuentes (con base en los criterios de ordenamiento)
    std::cout << "Los " << nColores << " colores menos frecuentes:\n";
    for (size_t i = 0; i < std::min(static_cast<size_t>(nColores), colorData.size()); i++) {
        auto [frequency, r, g, b] = colorData[i];
        std::cout << "Color RGB(" << r << ", " << g << ", " << b << ") - Frecuencia: " << frequency << '\n';
    }
    // Ya tenemos los nColores menos frecuentes
    // Dividimos los colores en los que se van a eliminar y los que se mantienen
    std::vector<std::tuple<int, int, int, int>> const colorsToRemove(colorData.begin(), colorData.begin() + nColores);
    std::vector<std::tuple<int, int, int, int>> const colorsToKeep(colorData.begin() + nColores, colorData.end());

    // Calcula el color más cercano para cada color a eliminar
    for (const auto& [freqRem, rRem, gRem, bRem] : colorsToRemove) {
        double minDistance = std::numeric_limits<double>::max();
        std::tuple<int, int, int, int> closestColor;

        for (const auto& [freqKeep, rKeep, gKeep, bKeep] : colorsToKeep) {
            double const distance = std::sqrt(std::pow(rRem - rKeep, 2) + std::pow(gRem - gKeep, 2) + std::pow(bRem - bKeep, 2));

            if (distance < minDistance) {
                minDistance = distance;
                closestColor = {freqKeep, rKeep, gKeep, bKeep};
            }
        }

        // Imprime el color a eliminar y el color más cercano a él
        auto [closestFreq, closestR, closestG, closestB] = closestColor;
        std::cout << "Color a eliminar: RGB(" << rRem << ", " << gRem << ", " << bRem << ") - Frecuencia: " << freqRem << '\n';
        std::cout << "Color más cercano: RGB(" << closestR << ", " << closestG << ", " << closestB << ") - Frecuencia: " << closestFreq << '\n';
        std::cout << "Distancia euclidiana: " << minDistance << '\n';
    }

    // Abre el archivo de salida y escribe la frecuencia de cada color
    std::ofstream outputFile(outputPath);
    if (!outputFile.is_open()) {
        std::cerr << "Error al abrir el archivo de salida: " << outputPath << '\n';
        return;
    }

    outputFile << "Color (r,g,b), Frecuencia\n";
    for (const auto& entry : colorFrequency) {
        outputFile << entry.first << ", " << entry.second << "\n";
    }

    outputFile.close();
    std::cout << "Archivo de salida generado: " << outputPath << '\n';
    std::cout << "(DEPURACION) LLamando a todos los argumentos para evitar errores de clangtidy" << metadata.width<< nColores<<'\n';
}*/
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
/*
// Función para combinar RGB en un único valor entero
constexpr int combineRGB(int red, int green, int blue) {
    return (red << changeBits16) | (green << changeBits) | blue; // Desplazamos los bits para R, G y B
}
*/
template <typename T>
constexpr Color<T> extractRGB(int rgb) {
    T blue = rgb & MAX_PIXEL_VALUE;                  // Últimos 8 bits son B
    T green = (rgb >> changeBits) & MAX_PIXEL_VALUE; // Siguientes 8 bits son G
    T red = (rgb >> changeBits16) & MAX_PIXEL_VALUE; // Primeros 8 bits son R

    return Color<T>(red, green, blue);
}
/*
// Función para extraer los componentes RGB de un valor entero
constexpr void extractRGB(int rgb, int& red, int& green, int& blue) {
    blue = rgb & MAX_PIXEL_VALUE;             // Últimos 8 bits son B
    green = (rgb >> changeBits) & MAX_PIXEL_VALUE;      // Siguientes 8 bits son G
    red = (rgb >> changeBits16) & MAX_PIXEL_VALUE;     // Primeros 8 bits son R
}
*/
/*void cutfreq(ImageAOS& srcImage, const PPMMetadata& metadata, int nColores, const std::string& outputPath) {
    // Mapa para almacenar la frecuencia de cada color (r, g, b)
    std::unordered_map<int, int> colorFrequency; // Usamos int como clave

    // Procesamiento de píxeles dependiendo del tipo de píxel en ImageAOS
    if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels)) {
        const auto& pixels8 = std::get<std::vector<Pixel8>>(srcImage.pixels);
        for (const auto& pixel : pixels8) {
            int const key = combineRGB(pixel.r, pixel.g, pixel.b); // Genera la clave única
            colorFrequency[key]++;
        }
    } else if (std::holds_alternative<std::vector<Pixel16>>(srcImage.pixels)) {
        const auto& pixels16 = std::get<std::vector<Pixel16>>(srcImage.pixels);
        for (const auto& pixel : pixels16) {
            int const key = combineRGB(pixel.r, pixel.g, pixel.b); // Genera la clave única
            colorFrequency[key]++;
        }
    }

    // Vector para almacenar los colores con sus frecuencias
    std::vector<std::tuple<int, int, int, int>> colorData; // {frecuencia, r, g, b}

    // Convertimos cada entrada en colorFrequency a una tupla y la añadimos al vector colorData
    for (const auto& [key, frequency] : colorFrequency) {
        int red = 0;
        int green = 0;
        int blue = 0;
        extractRGB(key, red, green, blue); // Extrae R, G, B de la clave
        colorData.emplace_back(frequency, red, green, blue);
    }

    // Ordenamos el vector: primero por frecuencia ascendente, luego por b, g y r descendente
    std::sort(colorData.begin(), colorData.end(), [](const auto& inicio, const auto& final) {
        if (std::get<0>(inicio) != std::get<0>(final)) { return std::get<0>(inicio) < std::get<0>(final);  // Frecuencia ascendente
}
        if (std::get<3>(inicio) != std::get<3>(final)) { return std::get<3>(inicio) > std::get<3>(final);  // b descendente
}
        if (std::get<2>(inicio) != std::get<2>(final)) { return std::get<2>(inicio) > std::get<2>(final);  // g descendente
}
        return std::get<1>(inicio) > std::get<1>(final);                                        // r descendente
    });

    // Dividimos los colores en los que se van a eliminar y los que se mantienen
    std::vector<std::tuple<int, int, int, int>> const colorsToRemove(colorData.begin(), colorData.begin() + nColores);
    std::vector<std::tuple<int, int, int, int>> const colorsToKeep(colorData.begin() + nColores, colorData.end());

    // Mapa de sustitución para reemplazar colores eliminados por el color más cercano
    std::unordered_map<int, std::tuple<int, int, int>> replacementMap;

    // Calcula el color más cercano para cada color a eliminar
    for (const auto& [freqRem, rRem, gRem, bRem] : colorsToRemove) {
        double minDistance = std::numeric_limits<double>::max();
        std::tuple<int, int, int> closestColor;

        for (const auto& [freqKeep, rKeep, gKeep, bKeep] : colorsToKeep) {
            double const distance = std::sqrt(std::pow(rRem - rKeep, 2) + std::pow(gRem - gKeep, 2) + std::pow(bRem - bKeep, 2));

            if (distance < minDistance) {
                minDistance = distance;
                closestColor = std::make_tuple(rKeep, gKeep, bKeep); // Utiliza std::make_tuple
            }
        }

        auto [closestR, closestG, closestB] = closestColor;
        replacementMap[combineRGB(rRem, gRem, bRem)] = {closestR, closestG, closestB}; // Aquí se asigna correctamente
    }

    // Sustituir los colores en la imagen original
    if (std::holds_alternative<std::vector<Pixel8>>(srcImage.pixels)) {
        auto& pixels8 = std::get<std::vector<Pixel8>>(srcImage.pixels);
        for (auto& pixel : pixels8) {
            int const key = combineRGB(pixel.r, pixel.g, pixel.b); // Genera la clave única
            if (replacementMap.find(key) != replacementMap.end()) {
                auto [newR, newG, newB] = replacementMap[key];
                pixel.r = static_cast<uint8_t>(newR);
                pixel.g = static_cast<uint8_t>(newG);
                pixel.b = static_cast<uint8_t>(newB);
            }
        }
    } else if (std::holds_alternative<std::vector<Pixel16>>(srcImage.pixels)) {
        auto& pixels16 = std::get<std::vector<Pixel16>>(srcImage.pixels);
        for (auto& pixel : pixels16) {
            int const key = combineRGB(pixel.r, pixel.g, pixel.b); // Genera la clave única
            if (replacementMap.find(key) != replacementMap.end()) {
                auto [newR, newG, newB] = replacementMap[key];
                pixel.r = static_cast<uint16_t>(newR);
                pixel.g = static_cast<uint16_t>(newG);
                pixel.b = static_cast<uint16_t>(newB);
            }
        }
    }

    std::cout << "Colores reemplazados en la imagen.\n";
    std::cout << "(DEPURACION) LLamando a todos los argumentos para evitar errores de clangtidy" << metadata.width<< outputPath<<'\n';
    saveToPPM(srcImage, metadata, outputPath); // Guardar la imagen modificada
}
*/
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
}
