//
// Created by mapor on 29/10/2024.
//

#include "compress.hpp"


// Determina el tamaño del píxel según el número de colores
size_t determinePixelSize(const size_t colorCount) {
    if (colorCount <= MAX_COLOR) {
        return 1;
    } if (colorCount <= MAX_16) {
        return 2;
    }
    return 4;
}

// Escribe un arreglo de bytes en el archivo
void writeBytes(std::ofstream& file, const std::vector<uint8_t>& buffer) {
    /*std::cout << "Escribiendo bytes: ";
    for (const uint8_t& byte : buffer) {
        std::cout << static_cast<int>(byte) << " ";  // Convertir el byte a int para imprimir el valor numérico
    }
    std::cout << '\n';*/

    // Convertir explícitamente de uint8_t a char
    std::vector<char> charBuffer(buffer.begin(), buffer.end());

    // Escribir los datos en el archivo como char
    file.write(charBuffer.data(), static_cast<std::streamsize>(charBuffer.size()));
}

// Escribe la tabla de colores en el archivo
void writeColorTable(std::ofstream& file, const std::vector<std::tuple<int, int, int>>& colorList, size_t colorBytes) {
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
            std::vector<uint8_t> const buffer(rgb.begin(), rgb.end());
            writeBytes(file, buffer); // Escribe los bytes en el archivo
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

            // Aquí cambiamos el tipo de buffer a uint8_t en lugar de char
            std::vector<uint8_t> buffer(rgb.size());  // Cambiado a uint8_t
            std::ranges::copy(rgb, buffer.begin());
            writeBytes(file, buffer); // Escribe los bytes en el archivo
        }
    }
}
// Escribe la secuencia de píxeles en el archivo como índices de la tabla de colores
void writePixelData(std::ofstream& file, const PixelData& pixelData, size_t pixelSize) {
    size_t const numPixels = pixelData.redChannel.size();
    for (size_t i = 0; i < numPixels; ++i) {
        int const rSequence = pixelData.redChannel[i];
        int const gSequence = pixelData.greenChannel[i];
        int const bSequence = pixelData.blueChannel[i];
        auto color = std::make_tuple(rSequence, gSequence, bSequence);
        uint32_t const index = pixelData.colorTable.at(color);

        if (pixelSize == 1) {
            auto const indexByte = static_cast<uint8_t>(index);
            std::vector<uint8_t> buffer(1);  // Cambié a uint8_t
            buffer[0] = indexByte;
            writeBytes(file, buffer); // Escribe los bytes en el archivo
        } else if (pixelSize == 2) {
            std::array<uint8_t, 2> indexBytes = {
                static_cast<uint8_t>(index & MAX_PIXEL_VALUE),       // LSB
                static_cast<uint8_t>(index >> BYTE_SIZE)             // MSB
            };
            std::vector<uint8_t> buffer(indexBytes.size());  // Cambié a uint8_t
            std::ranges::copy(indexBytes, buffer.begin());
            writeBytes(file, buffer); // Escribe los bytes en el archivo
        } else {
            std::array<uint8_t, 4> indexBytes = {
                static_cast<uint8_t>(index & MAX_PIXEL_VALUE),
                static_cast<uint8_t>((index >> BYTE_SIZE) & MAX_PIXEL_VALUE),
                static_cast<uint8_t>((index >> (BYTE_SIZE*2)) & MAX_PIXEL_VALUE),
                static_cast<uint8_t>((index >> (BYTE_SIZE*3)) & MAX_PIXEL_VALUE)
            };
            std::vector<uint8_t> buffer(indexBytes.size());  // Cambié a uint8_t
            std::ranges::copy(indexBytes, buffer.begin());
            writeBytes(file, buffer); // Escribe los bytes en el archivo
        }
    }
}


// Función para preparar los canales de color y la tabla de colores
PixelData prepareColorChannels(const ImageSOA& image, std::unordered_map<std::tuple<int, int, int>, uint32_t, TupleHash>& colorTable) {
    bool const is8bit = std::holds_alternative<std::vector<uint8_t>>(image.redChannel);
    std::vector<uint8_t> redChannel;
    std::vector<uint8_t> greenChannel;
    std::vector<uint8_t> blueChannel;

    if (is8bit) {
        redChannel = std::get<std::vector<uint8_t>>(image.redChannel);
        greenChannel = std::get<std::vector<uint8_t>>(image.greenChannel);
        blueChannel = std::get<std::vector<uint8_t>>(image.blueChannel);
    } else {
        redChannel = std::vector<uint8_t>(std::get<std::vector<uint16_t>>(image.redChannel).begin(),
                                           std::get<std::vector<uint16_t>>(image.redChannel).end());
        greenChannel = std::vector<uint8_t>(std::get<std::vector<uint16_t>>(image.greenChannel).begin(),
                                             std::get<std::vector<uint16_t>>(image.greenChannel).end());
        blueChannel = std::vector<uint8_t>(std::get<std::vector<uint16_t>>(image.blueChannel).begin(),
                                            std::get<std::vector<uint16_t>>(image.blueChannel).end());
    }

    size_t const numPixels = redChannel.size();

    // Construir la tabla de colores usando std::unordered_map
    unsigned int colorIndex = 0;
    for (size_t i = 0; i < numPixels; ++i) {
        int const red = redChannel[i];
        int const green = greenChannel[i];
        int const blue = blueChannel[i];
        auto color = std::make_tuple(red, green, blue);

        // Si el color no está en la tabla, agregarlo
        if (colorTable.find(color) == colorTable.end()) {
            colorTable[color] = (colorIndex++);
        }
    }

    // Devolver el objeto PixelData
    return PixelData{.redChannel=std::move(redChannel), .greenChannel=std::move(greenChannel), .blueChannel=std::move(blueChannel), .colorTable=colorTable};
}

// Función principal refactorizada
void writeCPPMSOA(const ImageSOA& image, const std::string& filename, const PPMMetadata& metadata) {
    // Abrir archivo en modo binario
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "No se pudo abrir el archivo para escritura.\n";
        return;
    }

    // Usar std::unordered_map para la tabla de colores
    std::unordered_map<std::tuple<int, int, int>, uint32_t, TupleHash> colorTable;

    // Preparar los canales de color y construir la tabla de colores
    PixelData const pixelData = prepareColorChannels(image, colorTable);

    // Crear un set a partir de las claves del colorTable (solo las claves, no los valores)
    std::set<std::tuple<int, int, int>> colorSet;
    for (const auto& entry : colorTable) {
        colorSet.insert(entry.first);  // Insertar solo la clave en el set
    }
    std::vector<std::tuple<int, int, int>> const colorList(colorSet.begin(), colorSet.end());
    size_t const colorCount = colorList.size();
    size_t const pixelSize = determinePixelSize(colorCount);  // Usar la función que determina el tamaño del píxel
    size_t const colorBytes = (metadata.max_value <= 255) ? 3 : 6;

    // Escribir el encabezado
    file << "C6 " << metadata.width << " " << metadata.height << " "
         << metadata.max_value << " " << colorCount << "\n";

    // Escribir la tabla de colores
    writeColorTable(file, colorList, colorBytes);

    // Escribir la secuencia de píxeles
    writePixelData(file, pixelData, pixelSize);

    file.close();
}