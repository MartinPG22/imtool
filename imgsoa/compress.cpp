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
        if (auto color = std::make_tuple(red, green, blue); !colorTable.contains(color)) {
            colorTable[color] = static_cast<uint32_t>(colorList.size());
            colorList.push_back(color);
        }
    }

    size_t const colorCount = colorList.size();
    size_t pixelSize = 0;
    if (constexpr size_t maxColorCount = 256; colorCount <= maxColorCount) {
        pixelSize = 1;
    } else if (colorCount <= MAX_16) {
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
            std::ranges::copy(rgb, buffer.begin());
            file.write(buffer.data(), static_cast<std::streamsize>(buffer.size())); // Conversión explícita
        } else {
            constexpr int size_6 = 6;
            constexpr int R_LSB = 0;
            constexpr int R_MSB = 1;
            constexpr int G_LSB = 2;
            constexpr int G_MSB = 3;
            constexpr int B_LSB = 4;
            constexpr int B_MSB = 5;
            std::array<uint8_t, size_6> rgb{};
            rgb[R_LSB] = static_cast<uint8_t>(rTabla & MAX_PIXEL_VALUE);       // LSB de R
            rgb[R_MSB] = static_cast<uint8_t>(rTabla >> BYTE_SIZE);    // MSB de R
            rgb[G_LSB] = static_cast<uint8_t>(gTabla & MAX_PIXEL_VALUE);       // LSB de G
            rgb[G_MSB] = static_cast<uint8_t>(gTabla >> BYTE_SIZE);    // MSB de G
            rgb[B_LSB] = static_cast<uint8_t>(bTabla & MAX_PIXEL_VALUE);       // LSB de B
            rgb[B_MSB] = static_cast<uint8_t>(bTabla >> BYTE_SIZE);    // MSB de B

            // Usar un buffer temporal
            std::vector<char> buffer(rgb.size());
            std::ranges::copy(rgb, buffer.begin());
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
                static_cast<uint8_t>(index & MAX_PIXEL_VALUE),       // LSB
                static_cast<uint8_t>(index >> BYTE_SIZE)          // MSB
            };

            // Usar un buffer temporal
            std::vector<char> buffer(indexBytes.size());
            std::ranges::copy(indexBytes, buffer.begin());
            file.write(buffer.data(), static_cast<std::streamsize>(buffer.size())); // Conversión explícita
        } else {
            // Para cuatro bytes
            std::array<uint8_t, 4> indexBytes = {
                static_cast<uint8_t>(index & MAX_PIXEL_VALUE),         // LSB
                static_cast<uint8_t>((index >> BYTE_SIZE) & MAX_PIXEL_VALUE),
                static_cast<uint8_t>((index >> BYTE_SIZE*2) & MAX_PIXEL_VALUE),
                static_cast<uint8_t>((index >> BYTE_SIZE*3) & MAX_PIXEL_VALUE)
            };

            // Usar un buffer temporal
            std::vector<char> buffer(indexBytes.size());
            std::ranges::copy(indexBytes, buffer.begin());
            file.write(buffer.data(), static_cast<std::streamsize>(buffer.size())); // Conversión explícita
        }
    }
    file.close();
}
