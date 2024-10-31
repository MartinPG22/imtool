//progargs.cpp
//
// Created by mapor on 07/10/2024.
//

#include "progargs.hpp"


bool isInteger(const std::string& integer) {
    return std::regex_match(integer, std::regex("-?[0-9]+"));
}
int executeInfo(const std::vector<std::string>& arguments, PPMMetadata& metadata) {
    const std::string& inputPath = arguments[0];
    const std::string& outputPath = arguments[1];
    const std::string& operation = arguments[2];
    std::vector<std::string> copia_arg;
    copia_arg.assign(arguments.begin()+3, arguments.end());

    if (arguments.size() != 3) {
        std::cerr << "Error: Invalid extra arguments for info: ";
        for (const auto& arg : copia_arg) {
            std::cerr << arg << " ";
        };
        std::cerr << '\n';
        return -1;
    }
    //execute modulo para el info
    std::cout << "(DEPURACION) Executing 'info' operation on: " << inputPath << '\n';
    std::cout << "Input: " << inputPath << '\n';
    std::cout << "Out: " << outputPath << '\n';
    std::cout << "Operation: " << operation << '\n';
    std::cout << "Image size: " << metadata.height << "x"<< metadata.width << '\n';
    std::cout << "Max level: " << metadata.max_value << '\n';
    return 0;
}

int executeMaxlevel(const std::vector<std::string>& arguments, PPMMetadata& metadata, const std::string& method) {
    const std::string& inputPath = arguments[0];
    const std::string& outputPath = arguments[1];
    const std::string& operation = arguments[2];
    std::cout << "(DEPURACION) LLamando a todos los argumentos para evitar errores de clangtidy" << inputPath << outputPath << operation<<'\n';
    if (method == "soa") {
        ImageSOA const imagensrcSOA = cargarImagenPPMSOA(inputPath, metadata);
        imprimirImagenSOA(imagensrcSOA, metadata);
    } else if (method == "aos") {
        ImageAOS const imagensrcAOS = cargarImagenPPM(inputPath, metadata);
    }

    if (arguments.size() != 4) {
        std::cerr << "Error: Invalid number of extra arguments for maxlevel: " << arguments.size() - 3 << '\n';
        return -1;
    }
    try {
        int const newMaxLevel = std::stoi(arguments[3]);
        if(0>newMaxLevel || newMaxLevel>MAX_NEW_LEVEL){
            std::cerr << "Error: Invalid maxlevel: " << arguments[3] << '\n';
            return -1;
        }
    } catch (std::invalid_argument&) {
        std::cerr << "Error: Invalid maxlevel: " << arguments[3] << '\n';
        return -1;
    }
    std::cout << "Executing 'maxlevel' operation with level on: " << inputPath << '\n';
    maxlevel();
    return 0;
}

int argumentsResize(const std::vector<std::string>& arguments) {
    if (arguments.size() != ARG_RESIZE) {
        std::cerr << "Error: Invalid number of extra arguments for resize: "<< arguments.size()-3 << '\n';
        return -1;
    }
    if (!isInteger(arguments[3])) {
        std::cerr << "Error: Invalid resize width: " << arguments[3] << '\n';
        return -1; // Sale si no es un entero
    }
    try {
        int const newWidth = std::stoi(arguments[3]);
        if(0>newWidth){
            std::cerr << "Error: Invalid resize width: " << arguments[3] << '\n';
            return -1;
        }
    } catch (std::invalid_argument&) {
        std::cerr << "Error: Invalid resize width: " << arguments[3] << '\n';
        return -1;
    }
    if (!isInteger(arguments[4])) {
        std::cerr << "Error: Invalid resize height: " << arguments[4] << '\n';
        return -1; // Sale si no es un entero
    }
    try {
        int const newHeight = std::stoi(arguments[4]);
        if(0>newHeight){
            std::cerr << "Error: Invalid resize height: " << arguments[4] << '\n';
            return -1;
        }
    } catch (std::invalid_argument&) {
        std::cerr << "Error: Invalid resize height: " << arguments[4] << '\n';
        return -1;
    }
    std::cout << "(Depuracion) He pasado las condiciones de parametros " <<'\n';
    return 0;
}

int executeResize(const std::vector<std::string>& arguments, PPMMetadata& metadata, const std::string& method) {
    const std::string& inputPath = arguments[0];
    const std::string& outputPath = arguments[1];
    const std::string& operation = arguments[2];

    if (argumentsResize(arguments) ==-1){return -1;};
    std::cout << "(Depuracion) Llego al resize " << inputPath << '\n';
    std::vector<size_t> const newSize = {std::stoul(arguments[3]), std::stoul(arguments[4])};
    //
    std::cout << "(Depuration) Executing 'resize' operation on: " << inputPath << '\n';
    if (method == "aos") {
        ImageAOS const imagensrcAOS = cargarImagenPPM(inputPath, metadata);
        ImageAOS const resizedImage = resize(imagensrcAOS, metadata, newSize, outputPath);
    } else if (method == "soa") {
        ImageSOA const imagensrcSOA = cargarImagenPPMSOA(inputPath, metadata);
        //imprimirImagenSOA(imagensrcSOA, metadata);
        //ImageSOA resizedImage = resize(imagensrcSOA, metadata, newWidth, newHeight);
    }
    std::cout << "(DEPURACION) LLamando a todos los argumentos para evitar errores de clangtidy" << inputPath << outputPath << operation<<'\n';
    return 0;
}
int executeCutfreq(const std::vector<std::string>& arguments, PPMMetadata& metadata, const std::string& method) {
    const std::string& inputPath = arguments[0];
    const std::string& outputPath = arguments[1];
    const std::string& operation = arguments[2];

    if (arguments.size() != 4) {
        std::cerr << "Error: Invalid number of extra arguments for cutfreq: " << arguments.size() - 3 << '\n';
        return -1;
    }
    if (!isInteger(arguments[3])) {
        std::cerr << "Error: Invalid cutfreq: " << arguments[3] << '\n';
        return -1; // Sale si no es un entero
    }
    int const numberOfColors = std::stoi(arguments[3]);
    if (numberOfColors <= 0) {
        std::cerr << "Error: Invalid cutfreq: " << arguments[3] << '\n';
        return -1;
    }
    if (method == "aos") {
        ImageAOS imagensrcAOS = cargarImagenPPM(inputPath, metadata);
        cutfreq(imagensrcAOS,metadata,numberOfColors, outputPath);
    } else if (method == "soa") {
        //ImageSOA imagensrcSOA = cargarImagenPPMSOA(inputPath, metadata);
        // Contar frecuencias de cada canal
        //ColorFrequencies const freqs = contarFrecuencias(imagensrcSOA, metadata.width, metadata.height);

        // Reemplazar colores
        //cutfreqAOS(imagensrcSOA, metadata.width, freqs, metadata.height);

    }
    std::cout << "Executing 'cutfreq' operation with number of colors: " << numberOfColors << " on: " << inputPath << '\n';
    std::cout << "(DEPURACION) LLamando a todos los argumentos para evitar errores de clangtidy" << inputPath << outputPath << operation<<'\n';
    return 0;
}
int executeCompress(const std::vector<std::string>& arguments, PPMMetadata& metadata, const std::string& method){
    const std::string& inputPath = arguments[0];
    const std::string& outputPath = arguments[1];
    const std::string& operation = arguments[2];

    // Declara las variables antes de usarlas

    if (arguments.size() != 3) {
        std::cerr << "Error: Invalid extra arguments for compress: " << arguments.size() - 3 << '\n';
        return -1;
    }

    std::cout << "Executing 'compress' operation on: " << inputPath << '\n';
    if (method == "aos") {
        ImageAOS const imagensrcAOS = cargarImagenPPM(inputPath, metadata);
        writeCPPM(imagensrcAOS, outputPath,metadata);
    } else if (method == "soa") {
        ImageSOA const imagensrcSOA = cargarImagenPPMSOA(inputPath, metadata);
        //imprimirImagenSOA(imagensrcSOA, metadata);
        //ImageSOA resizedImage = resize(imagensrcSOA, metadata, newWidth, newHeight);
    }
    std::cout << "(DEPURACION) LLamando a todos los argumentos para evitar errores de clangtidy" << inputPath << outputPath << operation<<'\n';
    return 0;
}

int executeOperation(const std::vector<std::string>& arguments,const std::string& method) {
    if (arguments.size() <3) {
        std::cerr << "Error: Invalid number of arguments: " << arguments.size() << '\n';
        return -1;
    }
    const std::string& inputPath = arguments[0];
    const std::string& operation = arguments[2];

    if (operation != "info" && operation != "maxlevel" && operation != "resize" && operation != "cutfreq" && operation != "compress") {
        std::cerr << "Error: Invalid operation: " << operation << '\n';
        return -1;
    }
    PPMMetadata metadata = readPPMMetadata(inputPath);

    std::cout << "(Depuracion) Llego aqui " << inputPath << '\n';
    if (operation == "info") { executeInfo(arguments, metadata);
    } else if (operation == "maxlevel") {executeMaxlevel(arguments, metadata, method);
    } else if (operation == "resize") {executeResize(arguments, metadata, method);
    } else if (operation == "cutfreq") {executeCutfreq(arguments, metadata, method);
    } else if (operation == "compress") {executeCompress(arguments, metadata, method);
    } else {
        std::cerr << "Error: Invalid option: " << operation << '\n';
    }
    return 0;
}
