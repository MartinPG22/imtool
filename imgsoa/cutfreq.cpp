//
// Created by mapor on 31/10/2024.
//

#include "cutfreq.hpp"


/*
ColorFrequencies contarFrecuencias(const ImageSOA& imagen, int width, int height) {
  ColorFrequencies freqs;
  const auto& red = std::get<std::vector<uint8_t>>(imagen.redChannel);
  const auto& green = std::get<std::vector<uint8_t>>(imagen.greenChannel);
  const auto& blue = std::get<std::vector<uint8_t>>(imagen.blueChannel);

  for (int i = 0; i < width * height; ++i) {
    if (red[i] > 0) { freqs.redCount++;
}
    if (green[i] > 0) { freqs.greenCount++;
}
    if (blue[i] > 0) { freqs.blueCount++;
}
  }
  return freqs;
}

void cutfreqAOS(ImageSOA& imagen, int width, const ColorFrequencies& freqs, int height) {
  // Determinar el canal con la mayor y la menor frecuencia
  int const maxFreq = std::max({ freqs.redCount, freqs.greenCount, freqs.blueCount });
  int const minFreq = std::min({ freqs.redCount, freqs.greenCount, freqs.blueCount });

  auto& red = std::get<std::vector<uint8_t>>(imagen.redChannel);
  auto& green = std::get<std::vector<uint8_t>>(imagen.greenChannel);
  auto& blue = std::get<std::vector<uint8_t>>(imagen.blueChannel);

  uint8_t* sourceChannel = nullptr;
  uint8_t* targetChannel = nullptr;

  // Identificar el canal a reemplazar (menos frecuente) y el canal de reemplazo (más frecuente)
  if (freqs.redCount == maxFreq) { sourceChannel = red.data();
  } else if (freqs.greenCount == maxFreq) { sourceChannel = green.data();
  } else { sourceChannel = blue.data();
}

  if (freqs.redCount == minFreq) { targetChannel = red.data();
  } else if (freqs.greenCount == minFreq) { targetChannel = green.data();
  } else { targetChannel = blue.data();
}

  // Reemplazar los valores en el canal menos frecuente con los valores del canal más frecuente
  for (int i = 0; i < width * height; ++i) {
    targetChannel[i] = sourceChannel[i];
  }
}
*/