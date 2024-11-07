#!/bin/sh
# Medir rendimiento general
perf stat -e cpu-cycles,instructions,cache-references,cache-misses \
    PracticaArcos2-release/imtool-aos/imtool-aos \
    PracticaArcos2/imagesPPM/lake-small.ppm \
    PracticaArcos2/imagesPPM/lake-small-aos-resize-8000.ppm \
    resize 8000 8000

echo "Medición de rendimiento finalizada"

# Medir consumo de energía
perf stat -e power/energy-cores/,power/energy-gpu/,power/energy-pkg/,power/energy-ram/ \
    PracticaArcos2-release/imtool-aos/imtool-aos \
    PracticaArcos2/imagesPPM/lake-small.ppm \
    PracticaArcos2/imagesPPM/lake-small-aos-resize-8000.ppm \
    resize 8000 8000

echo "Medición de energía finalizada"
