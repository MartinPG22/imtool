//
// Created by asanch on 11/2/24.
//

#include "maxlevel.hpp"


/**
 * @brief Cambia el nivel máximo de intensidad de una imagen en formato SOA.
 *
 * Esta función cambia el nivel máximo de intensidad de una imagen en formato SOA.
 *
 * @param srcImage La imagen en formato SOA.
 * @param metadata Metadatos de la imagen.
 * @param newMaxLevel El nuevo nivel máximo de intensidad.
 * @param outputPath La ruta donde se guardará la imagen con el nuevo nivel máximo.
 * @return int 0 si la imagen se guardó correctamente, 1 si hubo un error al abrir el archivo o
 *             si el formato de píxeles no es compatible.
 * @throws std::runtime_error Si no se puede abrir el archivo de salida o si el formato de píxeles no es compatible.
 */
int maxlevelSOA(const ImageSOA& srcImage, const PPMMetadata& metadata, const int newMaxLevel, const std::string& outputPath) {
    // Verificar que los canales tengan el mismo tamaño
    const auto& red = std::get<std::vector<uint8_t>>(srcImage.redChannel);
    const auto& green = std::get<std::vector<uint8_t>>(srcImage.greenChannel);
    const auto& blue = std::get<std::vector<uint8_t>>(srcImage.blueChannel);
    if (red.size() != green.size() || red.size() != blue.size()) {
        std::cerr << "Error: Los canales de color no tienen el mismo tamaño\n";
    }
    std::cout << "Cambiando el nivel máximo de intensidad de la imagen a " << newMaxLevel << '\n';
    // Convertir los valores de los píxeles
    std::vector<uint8_t> outRed(red.size());
    std::vector<uint8_t> outGreen(green.size());
    std::vector<uint8_t> outBlue(blue.size());
    for (size_t i = 0; i < red.size(); ++i) {
        outRed[i] = static_cast<uint8_t>(red[i] * newMaxLevel / metadata.max_value);
        outGreen[i] = static_cast<uint8_t>(green[i] * newMaxLevel / metadata.max_value);
        outBlue[i] = static_cast<uint8_t>(blue[i] * newMaxLevel / metadata.max_value);
    }
    // Guardar la imagen
    const ImageSOA max_level_SOA = { .redChannel=outRed, .greenChannel=outGreen, .blueChannel=outBlue };
    const PPMMetadata new_metadata = { .width=metadata.width, .height=metadata.height, .max_value=newMaxLevel };
    return saveSOAtoPPM(max_level_SOA, new_metadata, newMaxLevel, outputPath);
}
