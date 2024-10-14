//
// Created by mapor on 07/10/2024.
//

#include <iostream>
#include <string>
#include <vector>
#include <span>
#include "../common/progargs.cpp"  // Incluir la cabecera de la función

/*
 int main(int argc, char* argv[]) {
  // Convertir argc a std::size_t para evitar la advertencia
  gsl::span<char*> args_view(argv, static_cast<std::size_t>(argc));

  // Crear un vector de std::string, omitiendo el nombre del programa (argv[0])
  std::vector<std::string> arguments{args_view.begin() + 1, args_view.end()};

  // Ejecutar la operación usando los argumentos
  executeOperation(arguments);

  return 0;
}*/
int main(int argc, char* argv[]) {
  // Convertir argc a std::size_t para evitar la advertencia
  std::span const args_view(argv, static_cast<std::size_t>(argc));

  // Crear un vector de std::string, omitiendo el nombre del programa (argv[0])
  std::vector<std::string> arguments{args_view.begin() + 1, args_view.end()};

  // Ejecutar la operación usando los argumentos
  executeOperation(arguments);

  return 0;
}
