//
// Created by mapor on 31/10/2024.
//

#include "cutfreq.hpp"

ColorFrequencies contarFrecuencias(const ImageSOA& imagen, int width, int height) {
  ColorFrequencies freqs;
  const auto& red = std::get<std::vector<uint8_t>>(imagen.redChannel);
  const auto& green = std::get<std::vector<uint8_t>>(imagen.greenChannel);
  const auto& blue = std::get<std::vector<uint8_t>>(imagen.blueChannel);

  size_t const pixelCount = static_cast<size_t>(width) * static_cast<size_t>(height); // Multiplicación segura
  for (size_t i = 0; i < pixelCount; ++i) { // Usamos size_t
    if (red[i] > 0) { freqs.redCount++; }
    if (green[i] > 0) { freqs.greenCount++; }
    if (blue[i] > 0) { freqs.blueCount++; }
  }
  return freqs;
}

void cutfreqSOA(const ImageSOA& imagen, const PPMMetadata& metadata, const std::string& outputPath, const ColorFrequencies& freqs ) {
  // Determinar el canal con la mayor y la menor frecuencia
  int const maxFreq = std::max({ freqs.redCount, freqs.greenCount, freqs.blueCount });
  int const minFreq = std::min({ freqs.redCount, freqs.greenCount, freqs.blueCount });

  auto red = std::get<std::vector<uint8_t>>(imagen.redChannel);
  auto green = std::get<std::vector<uint8_t>>(imagen.greenChannel);
  auto blue = std::get<std::vector<uint8_t>>(imagen.blueChannel);

  // Determinar qué canal es el de origen y cuál es el de destino
  const std::vector<uint8_t>* sourceChannel = nullptr;
  std::vector<unsigned char> *targetChannel = nullptr;

  if (freqs.redCount == maxFreq) { sourceChannel = &red;
  } else if (freqs.greenCount == maxFreq) { sourceChannel = &green;
  } else { sourceChannel = &blue; }

  if (freqs.redCount == minFreq) { targetChannel = &red;
  } else if (freqs.greenCount == minFreq) { targetChannel = &green;
  } else { targetChannel = &blue; }
  size_t const pixelCount = static_cast<size_t>(metadata.width) * static_cast<size_t>(metadata.height); // Multiplicación segura
  // Reemplazar los valores del canal menos frecuente con los del canal más frecuente
  for (size_t i = 0; i < pixelCount; ++i) { // Usamos size_t
    (*targetChannel)[i] = (*sourceChannel)[i];
  }
    saveImageSOAToPPM(imagen, metadata, outputPath);
}

struct ColorChannels8Bit {
    std::vector<uint8_t> red;
    std::vector<uint8_t> green;
    std::vector<uint8_t> blue;
};
struct ColorChannels16Bit {
    std::vector<uint16_t> red;
    std::vector<uint16_t> green;
    std::vector<uint16_t> blue;
};
void writePPMHeader(std::ofstream& outFile, const PPMMetadata& metadata) {
    outFile << "P6\n" << metadata.width << " " << metadata.height << "\n" << metadata.max_value << "\n";
}
void writeData8Bit(std::ofstream& outFile, const ColorChannels8Bit& channels, const size_t pixelCount) {
    for (size_t i = 0; i < pixelCount; ++i) {
        outFile.put(static_cast<char>(channels.red[i]));
        outFile.put(static_cast<char>(channels.green[i]));
        outFile.put(static_cast<char>(channels.blue[i]));
    }
}
void writeData16Bit(std::ofstream& outFile, const ColorChannels16Bit& channels, const size_t pixelCount) {
    for (size_t i = 0; i < pixelCount; ++i) {
        outFile.put(static_cast<char>(channels.red[i] >> BYTE_SIZE));      // Byte alto
        outFile.put(static_cast<char>(channels.red[i] & HEX_VAL));         // Byte bajo
        outFile.put(static_cast<char>(channels.green[i] >> BYTE_SIZE));    // Byte alto
        outFile.put(static_cast<char>(channels.green[i] & HEX_VAL));       // Byte bajo
        outFile.put(static_cast<char>(channels.blue[i] >> BYTE_SIZE));     // Byte alto
        outFile.put(static_cast<char>(channels.blue[i] & HEX_VAL));        // Byte bajo
    }
}
void saveImageSOAToPPM(const ImageSOA& image, const PPMMetadata& metadata, const std::string& outputPath) {
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error al abrir el archivo para escritura: " << outputPath << '\n';
        return;
    }

    writePPMHeader(outFile, metadata);

    size_t const pixelCount = static_cast<size_t>(metadata.width) * metadata.height;

    if (std::holds_alternative<std::vector<uint8_t>>(image.redChannel) &&
        std::holds_alternative<std::vector<uint8_t>>(image.greenChannel) &&
        std::holds_alternative<std::vector<uint8_t>>(image.blueChannel)) {

        ColorChannels8Bit const channels {
            .red=std::get<std::vector<uint8_t>>(image.redChannel),
            .green=std::get<std::vector<uint8_t>>(image.greenChannel),
            .blue=std::get<std::vector<uint8_t>>(image.blueChannel)
        };
        writeData8Bit(outFile, channels, pixelCount);

    } else if (std::holds_alternative<std::vector<uint16_t>>(image.redChannel) &&
               std::holds_alternative<std::vector<uint16_t>>(image.greenChannel) &&
               std::holds_alternative<std::vector<uint16_t>>(image.blueChannel)) {

        ColorChannels16Bit const channels {
            .red=std::get<std::vector<uint16_t>>(image.redChannel),
            .green=std::get<std::vector<uint16_t>>(image.greenChannel),
            .blue=std::get<std::vector<uint16_t>>(image.blueChannel)
        };
        writeData16Bit(outFile, channels, pixelCount);

    } else {
        std::cerr << "Error: Tipo de canal de color no compatible.\n";
        return;
    }

    outFile.close();
    std::cout << "Imagen guardada en: " << outputPath << '\n';
}
