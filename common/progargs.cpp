//progargs.cpp
//
// Created by mapor on 07/10/2024.
//

#include "progargs.hpp"


bool isInteger(const std::string& integer) {
    return std::regex_match(integer, std::regex("-?[0-9]+"));
}
int executeInfo(const std::vector<std::string>& arguments, const PPMMetadata& metadata) {
    const std::string& inputPath = arguments[0];
    const std::string& outputPath = arguments[1];
    const std::string& operation = arguments[2];
    // Para poder sacar los argumentos de info e imprimirlos en caso de error
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
    std::cout << "Executing 'info' operation on: " << inputPath << '\n';
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

    if (method == "soa") {
        ImageSOA const imagensrcSOA = cargarImagenPPMtoSOA(inputPath, metadata);
    } else if (method == "aos") {
        ImageAOS const imagensrcAOS = cargarImagenPPMAOS(inputPath, metadata);
    }

    if (arguments.size() != 4) {
        std::cerr << "Error: Invalid number of extra arguments for maxlevel: " << arguments.size() - 3 << '\n';
        return -1;
    }
    try {
        if(int const newMaxLevel = std::stoi(arguments[3]); 0>newMaxLevel || newMaxLevel>MAX_NEW_LEVEL){
            std::cerr << "Error: Invalid maxlevel: " << arguments[3] << '\n';
            return -1;
        }
    } catch (std::invalid_argument&) {
        std::cerr << "Error: Invalid maxlevel: " << arguments[3] << '\n';
        return -1;
    }
    std::cout << "Executing 'maxlevel' operation with level on: " << inputPath << '\n';
    if (method == "aos") {
        ImageAOS const imagensrcAOS = cargarImagenPPMAOS(inputPath, metadata);
        ImageAOS const res_aos = maxlevelAOS(imagensrcAOS, metadata, std::stoi(arguments[3]), outputPath);
    } else if (method == "soa") {
        ImageSOA const imagensrcSOA = cargarImagenPPMtoSOA(inputPath, metadata);
        ImageSOA const res_soa = maxlevelSOA(imagensrcSOA, metadata, std::stoi(arguments[3]), outputPath);
    }
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
    return 0;
}

int executeResize(const std::vector<std::string>& arguments, PPMMetadata& metadata, const std::string& method) {
    const std::string& inputPath = arguments[0];
    const std::string& outputPath = arguments[1];

    if (argumentsResize(arguments) ==-1){return -1;};
    std::vector<size_t> const newSize = {std::stoul(arguments[3]), std::stoul(arguments[4])};
    //
    std::cout << "Executing 'resize' operation on: " << inputPath << '\n';
    if (method == "aos") {
        ImageAOS const imagensrcAOS = cargarImagenPPMAOS(inputPath, metadata);
        ImageAOS const resizedImage = resize(imagensrcAOS, metadata, newSize, outputPath);
    } else if (method == "soa") {
        ImageSOA const imagensrcSOA = cargarImagenPPMtoSOA(inputPath, metadata);
        ImageSOA const resizedImage = resizeSOA(imagensrcSOA, metadata, newSize, outputPath);
    }
    return 0;
}
int executeCutfreq(const std::vector<std::string>& arguments, PPMMetadata& metadata, const std::string& method) {
    const std::string& inputPath = arguments[0];
    const std::string& outputPath = arguments[1];

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
    std::cout << "Executing 'cutfreq' operation with number of colors: " << numberOfColors << " on: " << inputPath << '\n';
    if (method == "aos") {
        ImageAOS imagensrcAOS = cargarImagenPPMAOS(inputPath, metadata);
        cutfreq(imagensrcAOS,metadata,numberOfColors, outputPath);
    } else if (method == "soa") {
        // Load the image as SOA
        ImageSOA imagensrcSOA = cargarImagenPPMtoSOA(inputPath, metadata);

        // Apply cutfreq
        cutfreqSOA(imagensrcSOA, metadata, numberOfColors, outputPath);
    }
    return 0;
}
int executeCompress(const std::vector<std::string>& arguments, PPMMetadata& metadata, const std::string& method){
    const std::string& inputPath = arguments[0];
    const std::string& outputPath = arguments[1];

    if (arguments.size() != 3) {
        std::cerr << "Error: Invalid extra arguments for compress: " << arguments.size() - 3 << '\n';
        return -1;
    }

    std::cout << "Executing 'compress' operation on: " << inputPath << '\n';
    if (method == "aos") {
        ImageAOS const imagensrcAOS = cargarImagenPPMAOS(inputPath, metadata);
        writeCPPM(imagensrcAOS, outputPath,metadata);
    } else if (method == "soa") {
        ImageSOA const imagensrcSOA = cargarImagenPPMtoSOA(inputPath, metadata);
        //imprimirImagenSOA(imagensrcSOA, metadata);
        writeCPPMSOA(imagensrcSOA, outputPath, metadata);
    }
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
