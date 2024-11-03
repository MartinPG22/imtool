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
template <typename PixelType>
int maxlevelSOA(const ImageSOA<PixelType>& srcImage, const PPMMetadata& metadata, const int newMaxLevel, const std::string& outputPath) {
    // Check that the channels have the same size
    if (srcImage.redChannel.size() != srcImage.greenChannel.size() || 
        srcImage.redChannel.size() != srcImage.blueChannel.size()) {
        std::cerr << "Error: Color channels do not have the same size\n";
        return 1;
    }
    std::cout << "Cambiando el nivel máximo de intensidad de la imagen a " << newMaxLevel << '\n';
    // Create output channels
    std::vector<PixelType> outRed(srcImage.redChannel.size());
    std::vector<PixelType> outGreen(srcImage.greenChannel.size());
    std::vector<PixelType> outBlue(srcImage.blueChannel.size());
    // Convert pixel values
    for (size_t i = 0; i < srcImage.redChannel.size(); ++i) {
        outRed[i] = static_cast<PixelType>(srcImage.redChannel[i] * newMaxLevel / metadata.max_value);
        outGreen[i] = static_cast<PixelType>(srcImage.greenChannel[i] * newMaxLevel / metadata.max_value);
        outBlue[i] = static_cast<PixelType>(srcImage.blueChannel[i] * newMaxLevel / metadata.max_value);
    }
    // Guardar la imagen
    const ImageSOA<PixelType> max_level_SOA = { .redChannel=outRed, .greenChannel=outGreen, .blueChannel=outBlue };
    const PPMMetadata new_metadata = {.width=metadata.width, .height=metadata.height, .max_value=newMaxLevel};
    return saveSOAtoPPM(max_level_SOA, new_metadata, newMaxLevel, outputPath);
    //const ImageSOA max_level_SOA = { .redChannel=outRed, .greenChannel=outGreen, .blueChannel=outBlue };
    //const PPMMetadata new_metadata = { .width=metadata.width, .height=metadata.height, .max_value=newMaxLevel };
    //return saveSOAtoPPM(max_level_SOA, new_metadata, newMaxLevel, outputPath);
}

// Explicit instantiation
template int maxlevelSOA<uint8_t>(const ImageSOA<uint8_t>&, const PPMMetadata&, const int, const std::string&);
template int maxlevelSOA<uint16_t>(const ImageSOA<uint16_t>&, const PPMMetadata&, const int, const std::string&);
