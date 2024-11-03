//
// Created by mapor on 31/10/2024.
//

#include "cutfreq.hpp"

template <typename PixelType>
ColorFrequencies contarFrecuencias(const ImageSOA<PixelType> & imagen, int width, int height) {
  ColorFrequencies freqs;
  const auto& red = imagen.redChannel;
  const auto& green = imagen.greenChannel;
  const auto& blue = imagen.blueChannel;

  size_t const pixelCount = static_cast<size_t>(width) * static_cast<size_t>(height); // Multiplicación segura
  for (size_t i = 0; i < pixelCount; ++i) { // Usamos size_t
    if (red[i] > 0) { freqs.redCount++; }
    if (green[i] > 0) { freqs.greenCount++; }
    if (blue[i] > 0) { freqs.blueCount++; }
  }
  return freqs;
}

template <typename PixelType>
void cutfreqSOA(const ImageSOA<PixelType>& imagen, const PPMMetadata& metadata, const std::string& outputPath, const ColorFrequencies& freqs ) {
  // Determinar el canal con la mayor y la menor frecuencia
  int const maxFreq = std::max({ freqs.redCount, freqs.greenCount, freqs.blueCount });
  int const minFreq = std::min({ freqs.redCount, freqs.greenCount, freqs.blueCount });

  auto red = imagen.redChannel;
  auto green = imagen.greenChannel;
  auto blue = imagen.blueChannel;

  // Determinar qué canal es el de origen y cuál es el de destino
  std::vector<PixelType> sourceChannel;
  std::vector<PixelType> targetChannel;

  if (freqs.redCount == maxFreq) { sourceChannel = red;
  } else if (freqs.greenCount == maxFreq) { sourceChannel = green;
  } else { sourceChannel = blue; }

  if (freqs.redCount == minFreq) { targetChannel = red;
  } else if (freqs.greenCount == minFreq) { targetChannel = green;
  } else { targetChannel = blue; }
  size_t const pixelCount = static_cast<size_t>(metadata.width) * static_cast<size_t>(metadata.height); // Multiplicación segura
  // Reemplazar los valores del canal menos frecuente con los del canal más frecuente
  for (size_t i = 0; i < pixelCount; ++i) { // Usamos size_t
    targetChannel[i] = sourceChannel[i];
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

template <typename PixelType>
void saveImageSOAToPPM(const ImageSOA<PixelType> & image, const PPMMetadata& metadata, const std::string& outputPath) {
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error al abrir el archivo para escritura: " << outputPath << '\n';
        return;
    }

    writePPMHeader(outFile, metadata);

    size_t const pixelCount = static_cast<size_t>(metadata.width) * metadata.height;

    if constexpr (std::is_same_v<PixelType, uint8_t>) {
        // Use 8-bit channels directly
        ColorChannels8Bit const channels {
            .red = image.redChannel,
            .green = image.greenChannel,
            .blue = image.blueChannel
        };
        writeData8Bit(outFile, channels, pixelCount);

    } else if constexpr (std::is_same_v<PixelType, uint16_t>) {
        // Convert 16-bit channels to 8-bit
        ColorChannels16Bit const channels {
            .red = image.redChannel,
            .green = image.greenChannel,
            .blue = image.blueChannel
        };
        writeData16Bit(outFile, channels, pixelCount);

    } else {
        std::cerr << "Error: Tipo de canal de color no compatible.\n";
        return;
    }

    outFile.close();
    std::cout << "Imagen guardada en: " << outputPath << '\n';
}

// Explicit instantiation
template ColorFrequencies contarFrecuencias<uint8_t>(const ImageSOA<uint8_t>&, int, int);
template ColorFrequencies contarFrecuencias<uint16_t>(const ImageSOA<uint16_t>&, int, int);
template void cutfreqSOA<uint8_t>(const ImageSOA<uint8_t>&, const PPMMetadata&, const std::string&, const ColorFrequencies&);
template void cutfreqSOA<uint16_t>(const ImageSOA<uint16_t>&, const PPMMetadata&, const std::string&, const ColorFrequencies&);
template void saveImageSOAToPPM<uint8_t>(const ImageSOA<uint8_t>&, const PPMMetadata&, const std::string&);
template void saveImageSOAToPPM<uint16_t>(const ImageSOA<uint16_t>&, const PPMMetadata&, const std::string&);
