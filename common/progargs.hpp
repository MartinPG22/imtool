//
// Created by mapor on 14/10/2024.
//

#ifndef PROGARGS_HPP
#define PROGARGS_HPP


#include <iostream>
#include <string>
#include <vector>
#include <span>

void printUsage() {
    std::cerr << "Usage: imtool <input_path> <output_path> <operation> [<additional_args>...]" << std::endl;
}

void executeOperation(const std::vector<std::string>& arguments) {
    if (arguments.size() < 3) {
        printUsage();
        std::cerr << "Error: Invalid number of arguments." << std::endl;
        return;
    }

    std::string inputPath = arguments[0];
    std::string outputPath = arguments[1];
    std::string operation = arguments[2];

    if (operation == "info") {
        if (arguments.size() != 3) {
            std::cerr << "Error: Invalid extra arguments for info" << std::endl;
            return;
        }
        std::cout << "Executing 'info' operation on: " << inputPath << std::endl;

    } else if (operation == "maxlevel") {
        if (arguments.size() != 4) {
            std::cerr << "Error: Invalid number of extra arguments for maxlevel" << std::endl;
            return;
        }

        int newMaxLevel = std::stoi(arguments[3]);
        if (newMaxLevel < 0 || newMaxLevel > 65535) {
            std::cerr << "Error: Invalid maxlevel: " << arguments[3] << std::endl;
            return;
        }

        std::cout << "Executing 'maxlevel' operation with level: " << newMaxLevel << " on: " << inputPath << std::endl;

    } else if (operation == "resize") {
        if (arguments.size() != 5) {
            std::cerr << "Error: Invalid number of extra arguments for resize" << std::endl;
            return;
        }

        int newWidth = std::stoi(arguments[3]);
        int newHeight = std::stoi(arguments[4]);
        if (newWidth <= 0 || newHeight <= 0) {
            std::cerr << "Error: Invalid resize dimensions: " << newWidth << "x" << newHeight << std::endl;
            return;
        }

        std::cout << "Executing 'resize' operation with width: " << newWidth << " and height: " << newHeight << " on: " << inputPath << std::endl;

    } else if (operation == "cutfreq") {
        if (arguments.size() != 4) {
            std::cerr << "Error: Invalid number of extra arguments for cutfreq" << std::endl;
            return;
        }

        int numberOfColors = std::stoi(arguments[3]);
        if (numberOfColors <= 0) {
            std::cerr << "Error: Invalid cutfreq: " << arguments[3] << std::endl;
            return;
        }

        std::cout << "Executing 'cutfreq' operation with number of colors: " << numberOfColors << " on: " << inputPath << std::endl;

    } else if (operation == "compress") {
        if (arguments.size() != 3) {
            std::cerr << "Error: Invalid extra arguments for compress" << std::endl;
            return;
        }

        std::cout << "Executing 'compress' operation on: " << inputPath << std::endl;

    } else {
        std::cerr << "Error: Invalid option: " << operation << std::endl;
    }
}
#endif //PROGARGS_HPP
