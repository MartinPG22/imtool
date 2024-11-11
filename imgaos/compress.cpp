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
struct ColorCompare {
    bool operator()(const Pixel8& lhs, const Pixel8& rhs) const {
        return std::tie(lhs.r, lhs.g, lhs.b) < std::tie(rhs.r, rhs.g, rhs.b);
    }

    bool operator()(const Pixel16& lhs, const Pixel16& rhs) const {
        return std::tie(lhs.r, lhs.g, lhs.b) < std::tie(rhs.r, rhs.g, rhs.b);
    }
};
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

struct ColorData {
    std::unordered_map<Pixel8, uint8_t> pixelToIndex8;
    std::unordered_map<Pixel16, uint16_t> pixelToIndex16;
    std::set<Pixel8, ColorCompare> colorTable8;
    std::set<Pixel16, ColorCompare> colorTable16;
};

// Función para llenar el buffer con la tabla de colores y la secuencia de píxeles
namespace {
template <typename PixelType, typename IndexType>
void populateColorAndPixelBuffer(const std::vector<PixelType>& pixels,
                                 const std::unordered_map<PixelType, IndexType>& pixelToIndex,
                                 const std::set<PixelType, ColorCompare>& colorTable,
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
                          std::set<PixelType, ColorCompare>& colorTable) {
    for (const auto& pixel : pixels) {
        colorTable.insert(pixel); // Insertar el píxel (el orden se mantiene automáticamente)
    }

    IndexType index = 0;
    for (const auto& color : colorTable) {
        pixelToIndex[color] = index++; // Asignar índices en el orden de colorTable
    }
}
}
size_t writeHeader(std::ofstream& file, const PPMMetadata& metadata, size_t colorCount) {
    file << "C6 " << metadata.width << " " << metadata.height << " " << metadata.max_value << " " << colorCount << "\n";
    return colorCount;
}
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