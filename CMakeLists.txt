cmake_minimum_required(VERSION 3.22.1)
project(imtool LANGUAGES CXX)

# Print processor and compiler
message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set compiler options
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors -Wconversion -Wsign-conversion)  # Corrección aquí
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")  # Corrección aquí
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -funroll-loops")
# Support FetchContent functionality
include(FetchContent)
# Unit tests with google test
#Fetch googletest
FetchContent_Declare(googletest
        GIT_REPOSITORY "https://github.com/google/googletest"
        GIT_TAG "v1.15.2"
        GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(googletest)


# Enable GSL Library
FetchContent_Declare(GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG v4.0.0
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(GSL)

# Run clang-tidy on the whole source tree
# Note this will slow down compilation.
# You may temporarily disable but do not forget to enable again.
set(CMAKE_CXX_CLANG_TIDY clang-tidy;
        -format-style=file;
        -header-filter=.;)  # Corrección aquí

# All includes relative to source tree root.
include_directories(PUBLIC .)

# Process cmake from sim and fluid directories
add_subdirectory(common)
add_subdirectory(imgaos)
add_subdirectory(imgsoa)
add_subdirectory(imtool-aos)
add_subdirectory(imtool-soa)

# Unit tests and functional tests
enable_testing()
add_subdirectory(utest-common)
add_subdirectory(utest-imgaos)
add_subdirectory(utest-imagsoa)
add_subdirectory(ftest-aos)
add_subdirectory(ftest-soa)
