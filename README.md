# 📸 Imtool

**Imtool** es una herramienta de procesamiento de imágenes diseñada para trabajar con imágenes en formato PPM. Desarrollada como parte del curso de Arquitectura de Computadores en la Universidad Carlos III de Madrid, este proyecto se centra en optimizar transformaciones de imágenes utilizando **C++20**. Se exploran dos estrategias diferentes de organización de datos para evaluar su impacto en el rendimiento y la eficiencia energética.

---

## 🛠️ Implementaciones

El proyecto incluye dos versiones principales:

- **imtool-soa**: Utiliza el enfoque **Structure of Arrays (SOA)**.
- **imtool-aos**: Utiliza el enfoque **Array of Structures (AOS)**.

Estas implementaciones permiten comparar patrones de acceso a memoria y eficiencia computacional en tareas de procesamiento de imágenes.

---

## ✨ Funcionalidades

La aplicación permite realizar las siguientes operaciones en imágenes PPM:

1. **info**: Recupera metadatos de la imagen.
2. **maxlevel**: Escala el número de niveles de intensidad.
3. **resize**: Redimensiona la imagen utilizando interpolación bilineal.
4. **cutfreq**: Elimina los colores menos frecuentes de la imagen.
5. **compress**: Comprime la imagen utilizando un formato ficticio CPPM.

---

## 📂 Estructura del Proyecto

El proyecto está organizado en varias carpetas para mejorar la modularidad:

- **`common/`**: Código compartido por ambas implementaciones.
- **`imgsoa/`**: Código específico de la versión SOA.
- **`imgaos/`**: Código específico de la versión AOS.
- **`utest-common/`**: Pruebas unitarias para la biblioteca común.
- **`utest-imgsoa/`**: Pruebas unitarias para la biblioteca SOA.
- **`utest-imgaos/`**: Pruebas unitarias para la biblioteca AOS.
- **`ftest-soa/`**: Pruebas funcionales para imtool-soa.
- **`ftest-aos/`**: Pruebas funcionales para imtool-aos.
- **`imagesPPM/`**: Imágenes de prueba en formato PPM.

---

## 🏗️ Compilación e Instalación

El proyecto utiliza **CMake** para la compilación. Para compilar el proyecto, ejecuta los siguientes comandos desde la raíz del proyecto:

```bash
mkdir build
cd build
cmake ..
make
```

---

## 🚀Usage

To execute the application, use the following format:
```bash
./imtool-<soa|aos> <input.ppm> <output.ppm> <operation> [parameters]
```
Example usage:
```bash
./imtool-aos input.ppm output.ppm resize 200 150
````
---
## 📊Performance and Energy Evaluation

Performance and energy consumption tests have been conducted on the Avignon cluster using the perf tool.
---
## 🫂Project Members

- Alberto Sánchez del Álamo
- Amina Errami Maslaoui
- Martín Portugal González
- George Jose Hallak Pita


