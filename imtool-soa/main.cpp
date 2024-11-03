//
// Created by mapor on 19/10/2024.
//

#include "main.hpp"

int main(const int argc, char* argv[]) {
  // Convertir argc a std::size_t para evitar la advertencia
  std::span const args_view(argv, static_cast<std::size_t>(argc));

  // Crear un vector de std::string, omitiendo el nombre del programa (argv[0])
  std::vector<std::string> const arguments{args_view.begin() + 1, args_view.end()};

  // Ejecutar la operación usando los argumentos
  executeOperation(arguments, "soa");

  return 0;
}