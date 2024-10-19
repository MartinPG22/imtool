//progargs.cpp
//
// Created by mapor on 07/10/2024.
//
#include "binaryio.hpp"
#include "binaryio.cpp"
#include "./imgaos/imageaos.hpp"
#include "./imgaos/imageaos.cpp"

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <filesystem>  // Para trabajar con rutas de archivo


bool isInteger(const std::string& s) {
    return std::regex_match(s, std::regex("-?[0-9]+"));
}

int executeOperation(const std::vector<std::string>& arguments,const std::string& method) {
    if (arguments.size() <3) {
        std::cerr << "Error: Invalid number of arguments: " << arguments.size() << std::endl;
        return -1;
    }
    std::string inputPath = arguments[0];
    std::string outputPath = arguments[1];
    std::string operation = arguments[2];


    if (operation != "info" && operation != "maxlevel" && operation != "resize" && operation != "cutfreq" && operation != "compress") {
        std::cerr << "Error: Invalid operation: " << operation << std::endl;
        return -1;
    }

    std::vector<std::string> copia_arg;
    copia_arg.assign(arguments.begin()+3, arguments.end());
    PPMMetadata metadata = readPPMMetadata(inputPath);
    /* (DEPURACIÓN PARA COMPROBAR QUE SE TRANSFORMAN BIEN EN AOS)
    ImageAOS imagen = cargarImagenPPM(inputPath, metadata);
    imprimirPixeles(imagen, metadata);
    */
    if (operation == "info") {
        if (arguments.size() != 3) {
            std::cerr << "Error: Invalid extra arguments for info: ";
            for (const auto& arg : copia_arg) {
                std::cerr << arg << " ";
            };
            std::cerr << std::endl;
            return -1;
        }
        //execute modulo para el info
        std::cout << "(DEPURACION) Executing 'info' operation on: " << inputPath << std::endl;
        if (method == "aos") {
            std::cout << "Width: " << metadata.width << std::endl;
            std::cout << "Height: " << metadata.height << std::endl;
            std::cout << "Max value: " << metadata.max_value << std::endl;
        } else if (method == "soa") {
            std::cout << "Width: " << metadata.width << std::endl;
            std::cout << "Height: " << metadata.height << std::endl;
            std::cout << "Max value: " << metadata.max_value << std::endl;
        }
        // se puede eliminar este if para el info ya que se hace antes de la conversión de las imagenes a soa o aos
        // lo pongo ya que de momento no se usa el method en ningun lado, en el resto de funciones se deberia de usar

    } else if (operation == "maxlevel") {
        if (arguments.size() != 4) {
            std::cerr << "Error: Invalid number of extra arguments for maxlevel: " << arguments.size() - 3 << std::endl;
            return -1;
        }
        try {
            int newMaxLevel = std::stoi(arguments[3]);
            if(0>newMaxLevel || newMaxLevel>65535){
                std::cerr << "Error: Invalid maxlevel: " << arguments[3] << std::endl;
                return -1;
            }
        } catch (std::invalid_argument& e) {
            std::cerr << "Error: Invalid maxlevel: " << arguments[3] << std::endl;
            return -1;
        }
        std::cout << "Executing 'maxlevel' operation with level on: " << inputPath << std::endl;

    } else if (operation == "resize") {
        if (arguments.size() != 5) {
            std::cerr << "Error: Invalid number of extra arguments for resize: "<< arguments.size()-3 << std::endl;
            return -1;
        }
        if (!isInteger(arguments[3])) {
            std::cerr << "Error: Invalid resize width: " << arguments[3] << std::endl;
            return -1; // Sale si no es un entero
        }
        try {
            int newWidth = std::stoi(arguments[3]);
            if(0>newWidth){
                std::cerr << "Error: Invalid resize width: " << arguments[3] << std::endl;
                return -1;
            }
        } catch (std::invalid_argument& e) {
            std::cerr << "Error: Invalid resize width: " << arguments[3] << std::endl;
            return -1;
        }
        if (!isInteger(arguments[4])) {
            std::cerr << "Error: Invalid resize height: " << arguments[4] << std::endl;
            return -1; // Sale si no es un entero
        }
        try {
            int newHeight = std::stoi(arguments[4]);
            if(0>newHeight){
                std::cerr << "Error: Invalid resize height: " << arguments[4] << std::endl;
                return -1;
            }
        } catch (std::invalid_argument& e) {
            std::cerr << "Error: Invalid resize height: " << arguments[4] << std::endl;
            return -1;
        }

        std::cout << "Executing 'resize' operation on: " << inputPath << std::endl;

    } else if (operation == "cutfreq") {
        if (arguments.size() != 4) {
            std::cerr << "Error: Invalid number of extra arguments for cutfreq: " << arguments.size() - 3 << std::endl;
            return -1;
        }
        if (!isInteger(arguments[3])) {
            std::cerr << "Error: Invalid cutfreq: " << arguments[3] << std::endl;
            return -1; // Sale si no es un entero
        }
        int numberOfColors = std::stoi(arguments[3]);
        if (numberOfColors <= 0) {
            std::cerr << "Error: Invalid cutfreq: " << arguments[3] << std::endl;
            return -1;
        }

        std::cout << "Executing 'cutfreq' operation with number of colors: " << numberOfColors << " on: " << inputPath << std::endl;

    } else if (operation == "compress") {
        if (arguments.size() != 3) {
            std::cerr << "Error: Invalid extra arguments for compress: " << arguments.size() - 3 << std::endl;
            return -1;
        }

        std::cout << "Executing 'compress' operation on: " << inputPath << std::endl;

    } else {
        std::cerr << "Error: Invalid option: " << operation << std::endl;
    }
    return 0;
}

