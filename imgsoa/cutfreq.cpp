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

void cutfreqAOS(ImageSOA& imagen, const PPMMetadata& metadata, const std::string& outputPath, const ColorFrequencies& freqs ) {
  // Determinar el canal con la mayor y la menor frecuencia
  int const maxFreq = std::max({ freqs.redCount, freqs.greenCount, freqs.blueCount });
  int const minFreq = std::min({ freqs.redCount, freqs.greenCount, freqs.blueCount });

  auto& red = std::get<std::vector<uint8_t>>(imagen.redChannel);
  auto& green = std::get<std::vector<uint8_t>>(imagen.greenChannel);
  auto& blue = std::get<std::vector<uint8_t>>(imagen.blueChannel);

  // Determinar qué canal es el de origen y cuál es el de destino
  const std::vector<uint8_t>* sourceChannel = nullptr;
  std::vector<uint8_t>* targetChannel = nullptr;

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

// Función para guardar imagen SOA en formato PPM
void saveImageSOAToPPM(const ImageSOA& image, const PPMMetadata& metadata, const std::string& outputPath) {
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error al abrir el archivo para escritura: " << outputPath << '\n';
        return;
    }
    // puede que haya que recalcular el max_value ya que se eliminan colores. Pero habria que hacerlo una vez que se haya eliminado los colores
    // Escribir encabezado PPM
    outFile << "P6\n" << metadata.width << " " << metadata.height << "\n" << metadata.max_value << "\n";

    // Determinar el tipo de canal y escribir los datos de la imagen
    if (std::holds_alternative<std::vector<uint8_t>>(image.redChannel) &&
        std::holds_alternative<std::vector<uint8_t>>(image.greenChannel) &&
        std::holds_alternative<std::vector<uint8_t>>(image.blueChannel)) {

        // Obtener los canales de tipo uint8_t
        const auto& red = std::get<std::vector<uint8_t>>(image.redChannel);
        const auto& green = std::get<std::vector<uint8_t>>(image.greenChannel);
        const auto& blue = std::get<std::vector<uint8_t>>(image.blueChannel);

        // Escribir datos en formato binario (ppm tipo P6)
        for (size_t i = 0; i < static_cast<size_t>(metadata.width) * metadata.height; ++i) {
            outFile.put(static_cast<char>(red[i]));
            outFile.put(static_cast<char>(green[i]));
            outFile.put(static_cast<char>(blue[i]));
        }
    } else if (std::holds_alternative<std::vector<uint16_t>>(image.redChannel) &&
               std::holds_alternative<std::vector<uint16_t>>(image.greenChannel) &&
               std::holds_alternative<std::vector<uint16_t>>(image.blueChannel)) {

        // Obtener los canales de tipo uint16_t
        const auto& red = std::get<std::vector<uint16_t>>(image.redChannel);
        const auto& green = std::get<std::vector<uint16_t>>(image.greenChannel);
        const auto& blue = std::get<std::vector<uint16_t>>(image.blueChannel);

        // Escribir datos en formato binario
        for (size_t i = 0; i < static_cast<size_t>(metadata.width) * metadata.height; ++i) {
            outFile.put(static_cast<char>(red[i] >> BYTE_SIZE));      // Byte alto
            outFile.put(static_cast<char>(red[i] & HEX_VAL));    // Byte bajo
            outFile.put(static_cast<char>(green[i] >> BYTE_SIZE));    // Byte alto
            outFile.put(static_cast<char>(green[i] & HEX_VAL));  // Byte bajo
            outFile.put(static_cast<char>(blue[i] >> BYTE_SIZE));     // Byte alto
            outFile.put(static_cast<char>(blue[i] & HEX_VAL));   // Byte bajo
        }
    } else {
        std::cerr << "Error: Tipo de canal de color no compatible.\n";
        return;
    }

    outFile.close();
    std::cout << "Imagen guardada en: " << outputPath << '\n';
}