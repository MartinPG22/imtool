# 📸 Imtool

**Imtool** is an image processing tool designed to work with PPM format images. Developed as part of the Computer Architecture course at Universidad Carlos III de Madrid, this project focuses on optimizing image transformations using **C++20**. It explores two different data organization strategies to evaluate their impact on performance and energy efficiency.

---

## 🛠️ Implementations

The project includes two main versions:

- **imtool-soa**: Uses the **Structure of Arrays (SOA)** approach.
- **imtool-aos**: Uses the **Array of Structures (AOS)** approach.

These implementations allow for the comparison of memory access patterns and computational efficiency in image processing tasks.

---

## ✨ Features

The application supports the following operations on PPM images:

1. **info**: Retrieves image metadata.
2. **maxlevel**: Scales the number of intensity levels.
3. **resize**: Resizes the image using bilinear interpolation.
4. **cutfreq**: Removes the least frequent colors from the image.
5. **compress**: Compresses the image using a fictional CPPM format.

---

## 📂 Project Structure

The project is organized into several folders to improve modularity:

- **`common/`**: Code shared by both implementations.
- **`imgsoa/`**: Code specific to the SOA version.
- **`imgaos/`**: Code specific to the AOS version.
- **`utest-common/`**: Unit tests for the common library.
- **`utest-imgsoa/`**: Unit tests for the SOA library.
- **`utest-imgaos/`**: Unit tests for the AOS library.
- **`ftest-soa/`**: Functional tests for imtool-soa.
- **`ftest-aos/`**: Functional tests for imtool-aos.
- **`imagesPPM/`**: Test images in PPM format.

---

## 🏗️ Compilation and Installation

The project uses **CMake** for compilation. To compile the project, run the following commands from the project root:

```bash
mkdir build
cd build
cmake ..
make
````
---

## 🚀Usage

To execute the application, use the following format:

bash
./imtool-<soa|aos> <input.ppm> <output.ppm> <operation> [parameters]


Example usage:

bash
./imtool-aos input.ppm output.ppm resize 200 150


---
## 📊Performance and Energy Evaluation

Performance and energy consumption tests have been conducted on the Avignon cluster using the perf tool.
---
## 🫂Project Members

- Alberto Sánchez del Álamo
- Amina Errami Maslaoui
- Martín Portugal González
- George Jose Hallak Pita
