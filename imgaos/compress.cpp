//
// Created by mapor on 26/10/2024.
//

#include "compress.hpp"

// Sobrecarga de `operator==` para `Pixel8`
bool operator==(const Pixel8& lhs, const Pixel8& rhs) {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

// Sobrecarga de `operator==` para `Pixel16`
bool operator==(const Pixel16& lhs, const Pixel16& rhs) {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

// Hashing para permitir almacenar Pixeles en unordered_set
namespace std {
template<>
struct hash<Pixel8> {
    size_t operator()(const Pixel8& pixel) const noexcept { // Agregar noexcept
        return hash<uint8_t>()(pixel.r) ^ hash<uint8_t>()(pixel.g) ^ hash<uint8_t>()(pixel.b);
    }
};

template<>
struct hash<Pixel16> {
    size_t operator()(const Pixel16& pixel) const noexcept { // Agregar noexcept
        return hash<uint16_t>()(pixel.r) ^ hash<uint16_t>()(pixel.g) ^ hash<uint16_t>()(pixel.b);
    }
};
} // namespace std
/*
// Función para escribir el archivo en formato CPPM
void writeCPPM(const ImageAOS& image, const std::string& filename, const PPMMetadata& metadata) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo para escritura." << '\n';
        return;
    }
    // Mapa para almacenar los índices de colores
    std::unordered_map<Pixel8, uint8_t> pixelToIndex8;
    std::unordered_map<Pixel16, uint16_t> pixelToIndex16;

    // Conjuntos para almacenar colores únicos
    std::unordered_set<Pixel8> colorTable8;
    std::unordered_set<Pixel16> colorTable16;

    // Identificar y almacenar colores únicos según el tipo de píxel
    if (std::holds_alternative<std::vector<Pixel8>>(image.pixels)) {
        const auto& pixels = std::get<std::vector<Pixel8>>(image.pixels);
        for (const auto& pixel : pixels) {
            if (colorTable8.insert(pixel).second) { // Solo agrega si es único
                // Asegúrate de que no excedas el límite de 256 colores
                auto const index = static_cast<uint8_t>(colorTable8.size() - 1);
                pixelToIndex8[pixel] = index; // Mapea el pixel a un índice
            }
        }
    } else {
        const auto& pixels = std::get<std::vector<Pixel16>>(image.pixels);
        for (const auto& pixel : pixels) {
            if (colorTable16.insert(pixel).second) { // Solo agrega si es único
                // Asegúrate de que no excedas el límite de 65536 colores
                auto const index = static_cast<uint16_t>(colorTable16.size() - 1);
                pixelToIndex16[pixel] = index; // Mapea el pixel a un índice
            }
        }
    }

    // Determinar el número de colores únicos
    size_t const colorCount = std::holds_alternative<std::vector<Pixel8>>(image.pixels) ? colorTable8.size() : colorTable16.size();

    // Escribir la cabecera en formato CPPM
    file << "C6 " << metadata.width << " " << metadata.height << " " << metadata.max_value << " " << colorCount << "\n";

    // Escribir la tabla de colores
    if (std::holds_alternative<std::vector<Pixel8>>(image.pixels)) {
        for (const auto& color : colorTable8) {
            file.write(reinterpret_cast<const char*>(&color.r), sizeof(color.r));
            file.write(reinterpret_cast<const char*>(&color.g), sizeof(color.g));
            file.write(reinterpret_cast<const char*>(&color.b), sizeof(color.b));
        }
        // Escribir la secuencia de valores de píxeles
        const auto& pixels = std::get<std::vector<Pixel8>>(image.pixels);
        for (const auto& pixel : pixels) {
            uint8_t index = pixelToIndex8[pixel]; // Obtener el índice del color
            file.write(reinterpret_cast<const char*>(&index), sizeof(uint8_t)); // Escribir como 1 byte
        }
    } else {
        for (const auto& color : colorTable16) {
            file.write(reinterpret_cast<const char*>(&color.r), sizeof(color.r));
            file.write(reinterpret_cast<const char*>(&color.g), sizeof(color.g));
            file.write(reinterpret_cast<const char*>(&color.b), sizeof(color.b));
        }
        // Escribir la secuencia de valores de píxeles
        const auto& pixels = std::get<std::vector<Pixel16>>(image.pixels);
        for (const auto& pixel : pixels) {
            uint16_t index = pixelToIndex16[pixel]; // Obtener el índice del color
            file.write(reinterpret_cast<const char*>(&index), sizeof(uint16_t)); // Escribir como 2 bytes
        }
    }
    file.close();
    std::cout << "Archivo " << filename << " generado correctamente en formato CPPM." << '\n';
}
*/

// Función para llenar el buffer con la tabla de colores y la secuencia de píxeles
namespace {
template <typename PixelType, typename IndexType>
void populateColorAndPixelBuffer(const std::vector<PixelType>& pixels,
                                 const std::unordered_map<PixelType, IndexType>& pixelToIndex,
                                 const std::unordered_set<PixelType>& colorTable,
                                 std::vector<uint8_t>& buffer) {
    if constexpr (std::is_same_v<PixelType, Pixel8>) {
        // Procesar la tabla de colores para Pixel8
        for (const auto& color : colorTable) {
            buffer.push_back(color.r);
            buffer.push_back(color.g);
            buffer.push_back(color.b);
        }

        // Agregar la secuencia de índices de píxeles para Pixel8
        for (const auto& pixel : pixels) {
            uint8_t const index = pixelToIndex.at(pixel); // Obtener el índice del color
            buffer.push_back(index); // Escribir como 1 byte
        }
    } else {
        // Procesar la tabla de colores para Pixel16
        constexpr int changeBits = 8;
        constexpr int maxPixelValue = 0xFF;

        for (const auto& color : colorTable) {
            // Descomponer componentes de color en bytes y almacenarlos
            buffer.push_back(static_cast<uint8_t>(color.r >> changeBits)); // alto byte
            buffer.push_back(static_cast<uint8_t>(color.r & maxPixelValue)); // bajo byte
            buffer.push_back(static_cast<uint8_t>(color.g >> changeBits)); // alto byte
            buffer.push_back(static_cast<uint8_t>(color.g & maxPixelValue)); // bajo byte
            buffer.push_back(static_cast<uint8_t>(color.b >> changeBits)); // alto byte
            buffer.push_back(static_cast<uint8_t>(color.b & maxPixelValue)); // bajo byte
        }

        // Agregar la secuencia de índices de píxeles para Pixel16
        for (const auto& pixel : pixels) {
            uint16_t const index = pixelToIndex.at(pixel); // Obtener el índice del color
            buffer.push_back(static_cast<uint8_t>(index >> changeBits)); // alto byte
            buffer.push_back(static_cast<uint8_t>(index & maxPixelValue)); // bajo byte
        }
    }
    }
template <typename PixelType, typename IndexType>
void identifyUniqueColors(const std::vector<PixelType>& pixels,
                          std::unordered_map<PixelType, IndexType>& pixelToIndex,
                          std::unordered_set<PixelType>& colorTable) {
    for (const auto& pixel : pixels) {
        if (colorTable.insert(pixel).second) { // Solo agrega si es único
            auto const index = static_cast<IndexType>(colorTable.size() - 1);
            pixelToIndex[pixel] = index; // Mapea el pixel a un índice
        }
    }
}
}
/*
// Función para escribir el archivo en formato CPPM
void writeCPPM(const ImageAOS& image, const std::string& filename, const PPMMetadata& metadata) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo para escritura." << '\n';
        return;
    }

    // Mapa para almacenar los índices de colores
    std::unordered_map<Pixel8, uint8_t> pixelToIndex8;
    std::unordered_map<Pixel16, uint16_t> pixelToIndex16;

    // Conjuntos para almacenar colores únicos
    std::unordered_set<Pixel8> colorTable8;
    std::unordered_set<Pixel16> colorTable16;

    // Identificar y almacenar colores únicos según el tipo de píxel
    if (std::holds_alternative<std::vector<Pixel8>>(image.pixels)) {
        const auto& pixels = std::get<std::vector<Pixel8>>(image.pixels);
        for (const auto& pixel : pixels) {
            if (colorTable8.insert(pixel).second) { // Solo agrega si es único
                // Asegúrate de que no excedas el límite de 256 colores
                auto const index = static_cast<uint8_t>(colorTable8.size() - 1);
                pixelToIndex8[pixel] = index; // Mapea el pixel a un índice
            }
        }
    } else {
        const auto& pixels = std::get<std::vector<Pixel16>>(image.pixels);
        for (const auto& pixel : pixels) {
            if (colorTable16.insert(pixel).second) { // Solo agrega si es único
                // Asegúrate de que no excedas el límite de 65536 colores
                auto const index = static_cast<uint16_t>(colorTable16.size() - 1);
                pixelToIndex16[pixel] = index; // Mapea el pixel a un índice
            }
        }
    }

    // Determinar el número de colores únicos
    size_t const colorCount = std::holds_alternative<std::vector<Pixel8>>(image.pixels) ? colorTable8.size() : colorTable16.size();

    // Escribir la cabecera en formato CPPM
    file << "C6 " << metadata.width << " " << metadata.height << " " << metadata.max_value << " " << colorCount << "\n";

    // Buffer para colores y píxeles usando uint8_t
    std::vector<uint8_t> buffer;

    // Llamar a la función externa para llenar el buffer
    if (std::holds_alternative<std::vector<Pixel8>>(image.pixels)) {
        const auto& pixels = std::get<std::vector<Pixel8>>(image.pixels);
        populateColorAndPixelBuffer(pixels, pixelToIndex8, colorTable8, buffer);
    } else {
        const auto& pixels = std::get<std::vector<Pixel16>>(image.pixels);
        populateColorAndPixelBuffer(pixels, pixelToIndex16, colorTable16, buffer);
    }

    // Escribir el buffer de una vez
    std::vector<char> charBuffer(buffer.begin(), buffer.end());
    file.write(charBuffer.data(), static_cast<std::streamsize>(charBuffer.size()));
    file.close();
    std::cout << "Archivo " << filename << " generado correctamente en formato CPPM." << '\n';
}
*/


size_t writeHeader(std::ofstream& file, const PPMMetadata& metadata, size_t colorCount) {
    file << "C6 " << metadata.width << " " << metadata.height << " " << metadata.max_value << " " << colorCount << "\n";
    return colorCount;
}
// Estructura para almacenar los datos de colores
struct ColorData {
    std::unordered_map<Pixel8, uint8_t> pixelToIndex8;
    std::unordered_map<Pixel16, uint16_t> pixelToIndex16;
    std::unordered_set<Pixel8> colorTable8;
    std::unordered_set<Pixel16> colorTable16;
};

void writeCPPM(const ImageAOS& image, const std::string& filename, const PPMMetadata& metadata) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo para escritura." << '\n';
        return;
    }

    ColorData colordata;

    // Identificar y almacenar colores únicos según el tipo de píxel
    if (std::holds_alternative<std::vector<Pixel8>>(image.pixels)) {
        const auto& pixels = std::get<std::vector<Pixel8>>(image.pixels);
        identifyUniqueColors(pixels, colordata.pixelToIndex8, colordata.colorTable8);
    } else {
        const auto& pixels = std::get<std::vector<Pixel16>>(image.pixels);
        identifyUniqueColors(pixels, colordata.pixelToIndex16, colordata.colorTable16);
    }

    // Determinar el número de colores únicos
    size_t const colorCount = std::holds_alternative<std::vector<Pixel8>>(image.pixels) ? colordata.colorTable8.size() : colordata.colorTable16.size();

    // Escribir la cabecera en formato CPPM
    writeHeader(file, metadata, colorCount);

    // Buffer para colores y píxeles usando uint8_t
    std::vector<uint8_t> buffer;

    // Llamar a la función externa para llenar el buffer
    if (std::holds_alternative<std::vector<Pixel8>>(image.pixels)) {
        const auto& pixels = std::get<std::vector<Pixel8>>(image.pixels);
        populateColorAndPixelBuffer(pixels, colordata.pixelToIndex8, colordata.colorTable8,buffer);
    } else {
        const auto& pixels = std::get<std::vector<Pixel16>>(image.pixels);
        populateColorAndPixelBuffer(pixels, colordata.pixelToIndex16, colordata.colorTable16,buffer);
    }

    finish(buffer, file, filename);
}
void finish(std::vector<uint8_t> buffer, std::ofstream& file, const std::string& filename) {
    // Escribir el buffer de una vez
    std::vector<char> charBuffer(buffer.begin(), buffer.end());
    file.write(charBuffer.data(), static_cast<std::streamsize>(charBuffer.size()));
    file.close();
    std::cout << "Archivo " << filename << " generado correctamente en formato CPPM." << '\n';
}
