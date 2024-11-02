//
// Created by mapor on 29/10/2024.
//

#include "compress.hpp"


void writeCPPMSOA(const ImageSOA& image, const std::string& filename, const PPMMetadata& metadata) {
    // Abrir archivo en modo binario
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "No se pudo abrir el archivo para escritura.\n";
        return;
    }

    // Tabla de colores: color -> índice
    std::unordered_map<std::tuple<int, int, int>, uint32_t, TupleHash> colorTable;
    std::vector<std::tuple<int, int, int>> colorList;

    // Determinar el tipo de canal y obtener el tamaño
    size_t const numPixels = metadata.width * metadata.height;
    bool const is8bit = std::holds_alternative<std::vector<uint8_t>>(image.redChannel);
    std::vector<uint8_t> redChannel;
    std::vector<uint8_t> greenChannel;
    std::vector<uint8_t> blueChannel;

    if (is8bit) {
        redChannel = std::get<std::vector<uint8_t>>(image.redChannel);
        greenChannel = std::get<std::vector<uint8_t>>(image.greenChannel);
        blueChannel = std::get<std::vector<uint8_t>>(image.blueChannel);
    } else {
        // Si no es 8 bits, se usan los canales de 16 bits
        redChannel = std::vector<uint8_t>(std::get<std::vector<uint16_t>>(image.redChannel).begin(),
                                           std::get<std::vector<uint16_t>>(image.redChannel).end());
        greenChannel = std::vector<uint8_t>(std::get<std::vector<uint16_t>>(image.greenChannel).begin(),
                                             std::get<std::vector<uint16_t>>(image.greenChannel).end());
        blueChannel = std::vector<uint8_t>(std::get<std::vector<uint16_t>>(image.blueChannel).begin(),
                                            std::get<std::vector<uint16_t>>(image.blueChannel).end());
    }

    // Construir la tabla de colores
    for (size_t i = 0; i < numPixels; ++i) {
        int const red = redChannel[i];
        int const green = greenChannel[i];
        int const blue = blueChannel[i];
        auto color = std::make_tuple(red, green, blue);
        if (colorTable.find(color) == colorTable.end()) {
            colorTable[color] = static_cast<uint32_t>(colorList.size());
            colorList.push_back(color);
        }
    }

    size_t const colorCount = colorList.size();
    size_t pixelSize = 0;
    if (colorCount <= 256) {
        pixelSize = 1;
    } else if (colorCount <= 65536) {
        pixelSize = 2;
    } else {
        pixelSize = 4;
    }
    size_t const colorBytes = (metadata.max_value <= 255) ? 3 : 6;

    // Escribir el encabezado
    file << "C6 " << metadata.width << " " << metadata.height << " "
         << metadata.max_value << " " << colorCount << "\n";

    // Escribir la tabla de colores
    for (const auto& color : colorList) {
        int rTabla = 0;
        int gTabla = 0;
        int bTabla = 0;
        std::tie(rTabla, gTabla, bTabla) = color;

        if (colorBytes == 3) {
            std::array<uint8_t, 3> rgb = {
                static_cast<uint8_t>(rTabla),
                static_cast<uint8_t>(gTabla),
                static_cast<uint8_t>(bTabla)
            };

            // Usar un buffer temporal
            std::vector<char> buffer(rgb.size());
            std::copy(rgb.begin(), rgb.end(), buffer.begin());
            file.write(buffer.data(), static_cast<std::streamsize>(buffer.size())); // Conversión explícita
        } else {
            std::array<uint8_t, 6> rgb{};
            rgb[0] = static_cast<uint8_t>(rTabla & 0xFF);       // LSB de R
            rgb[1] = static_cast<uint8_t>(rTabla >> 8);        // MSB de R
            rgb[2] = static_cast<uint8_t>(gTabla & 0xFF);       // LSB de G
            rgb[3] = static_cast<uint8_t>(gTabla >> 8);        // MSB de G
            rgb[4] = static_cast<uint8_t>(bTabla & 0xFF);       // LSB de B
            rgb[5] = static_cast<uint8_t>(bTabla >> 8);        // MSB de B

            // Usar un buffer temporal
            std::vector<char> buffer(rgb.size());
            std::copy(rgb.begin(), rgb.end(), buffer.begin());
            file.write(buffer.data(), static_cast<std::streamsize>(buffer.size())); // Conversión explícita
        }
    }


    // Escribir la secuencia de píxeles como índices en la tabla de colores
    for (size_t i = 0; i < numPixels; ++i) {
        int const rSequence = redChannel[i];
        int const gSequence = greenChannel[i];
        int const bSequence = blueChannel[i];
        auto color = std::make_tuple(rSequence, gSequence, bSequence);
        uint32_t const index = colorTable[color];

        if (pixelSize == 1) {
            // Para un único byte
            // Para un único byte
            auto const indexByte = static_cast<uint8_t>(index);

            // Usar un buffer temporal para un solo byte
            std::vector<char> buffer(1);  // Crear un buffer de un solo byte
            buffer[0] = static_cast<char>(indexByte);  // Guardar el byte en el buffer
        } else if (pixelSize == 2) {
            // Para dos bytes
            std::array<uint8_t, 2> indexBytes = {
                static_cast<uint8_t>(index & 0xFF),       // LSB
                static_cast<uint8_t>(index >> 8)          // MSB
            };

            // Usar un buffer temporal
            std::vector<char> buffer(indexBytes.size());
            std::copy(indexBytes.begin(), indexBytes.end(), buffer.begin());
            file.write(buffer.data(), static_cast<std::streamsize>(buffer.size())); // Conversión explícita
        } else {
            // Para cuatro bytes
            std::array<uint8_t, 4> indexBytes = {
                static_cast<uint8_t>(index & 0xFF),         // LSB
                static_cast<uint8_t>((index >> 8) & 0xFF),
                static_cast<uint8_t>((index >> 16) & 0xFF),
                static_cast<uint8_t>((index >> 24) & 0xFF)
            };

            // Usar un buffer temporal
            std::vector<char> buffer(indexBytes.size());
            std::copy(indexBytes.begin(), indexBytes.end(), buffer.begin());
            file.write(buffer.data(), static_cast<std::streamsize>(buffer.size())); // Conversión explícita
        }
    }


    file.close();
}