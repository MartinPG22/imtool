//
// Created by mapor on 29/10/2024.
//
/*
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

    // Escribir la tabla de colores
    if (std::holds_alternative<std::vector<Pixel8>>(image.pixels)) {
        for (const auto& color : colorTable8) {
            buffer.push_back(color.r);
            buffer.push_back(color.g);
            buffer.push_back(color.b);
        }

        // Escribir la secuencia de valores de píxeles
        const auto& pixels = std::get<std::vector<Pixel8>>(image.pixels);
        for (const auto& pixel : pixels) {
            uint8_t const index = pixelToIndex8[pixel]; // Obtener el índice del color
            buffer.push_back(index); // Escribir como 1 byte
        }
    } else {
        for (const auto& color : colorTable16) {
            // Separa los componentes de color y los empuja como uint8_t
            buffer.push_back(static_cast<uint8_t>(color.r >> 8)); // alto byte
            buffer.push_back(static_cast<uint8_t>(color.r & 0xFF)); // bajo byte
            buffer.push_back(static_cast<uint8_t>(color.g >> 8)); // alto byte
            buffer.push_back(static_cast<uint8_t>(color.g & 0xFF)); // bajo byte
            buffer.push_back(static_cast<uint8_t>(color.b >> 8)); // alto byte
            buffer.push_back(static_cast<uint8_t>(color.b & 0xFF)); // bajo byte
        }

        // Escribir la secuencia de valores de píxeles
        const auto& pixels = std::get<std::vector<Pixel16>>(image.pixels);
        for (const auto& pixel : pixels) {
            uint16_t const index = pixelToIndex16[pixel]; // Obtener el índice del color
            buffer.push_back(static_cast<uint8_t>(index >> 8)); // alto byte
            buffer.push_back(static_cast<uint8_t>(index & 0xFF)); // bajo byte
        }
    }


    std::vector<char> charBuffer(buffer.begin(), buffer.end());
    file.write(charBuffer.data(), static_cast<std::streamsize>(charBuffer.size()));
    file.close();
    std::cout << "Archivo " << filename << " generado correctamente en formato CPPM." << '\n';
}
*/