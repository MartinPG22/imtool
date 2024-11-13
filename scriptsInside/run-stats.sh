#!/bin/sh
# Medir rendimiento general
echo "Resize lake-small 8000x8000"
echo "Resize AOS"
perf stat -e cpu-cycles,instructions,cache-references,cache-misses \
    PracticaArcos2-release/imtool-aos/imtool-aos \
    PracticaArcos2/imagesPPM/lake-small.ppm \
    PracticaArcos2/imagesPPM/lake-small-aos-resize-8000.ppm \
    resize 8000 8000

echo "Medición de rendimiento finalizada"

# Medir consumo de energía
perf stat -e power/energy-cores/,power/energy-pkg/,power/energy-ram/ \
    PracticaArcos2-release/imtool-aos/imtool-aos \
    PracticaArcos2/imagesPPM/lake-small.ppm \
    PracticaArcos2/imagesPPM/lake-small-aos-resize-8000.ppm \
    resize 8000 8000

echo "Medición de energía finalizada"

echo "Resize SOA"
perf stat -e cpu-cycles,instructions,cache-references,cache-misses \
    PracticaArcos2-release/imtool-soa/imtool-soa \
    PracticaArcos2/imagesPPM/lake-small.ppm \
    PracticaArcos2/imagesPPM/lake-small-soa-resize-8000.ppm \
    resize 8000 8000

echo "Medición de rendimiento finalizada"

# Medir consumo de energía
perf stat -e power/energy-cores/,power/energy-pkg/,power/energy-ram/ \
    PracticaArcos2-release/imtool-soa/imtool-soa \
    PracticaArcos2/imagesPPM/lake-small.ppm \
    PracticaArcos2/imagesPPM/lake-small-soa-resize-8000.ppm \
    resize 8000 8000

echo "Medición de energía finalizada"
echo "Fin Resize"
# ------------------------------------------------
echo "Maxlevel lake-large 65535"
echo "Maxlevel AOS"
perf stat -e cpu-cycles,instructions,cache-references,cache-misses \
    PracticaArcos2-release/imtool-aos/imtool-aos \
    PracticaArcos2/imagesPPM/lake-large.ppm \
    PracticaArcos2/imagesPPM/lake-large-aos-maxlevel-65535.ppm \
    maxlevel 65535

echo "Medición de rendimiento finalizada"

# Medir consumo de energía
perf stat -e power/energy-cores/,power/energy-pkg/,power/energy-ram/ \
    PracticaArcos2-release/imtool-aos/imtool-aos \
    PracticaArcos2/imagesPPM/lake-large.ppm \
    PracticaArcos2/imagesPPM/lake-large-aos-maxlevel-65535.ppm \
    maxlevel 65535

echo "Medición de energía finalizada"

echo "Maxlevel SOA"
perf stat -e cpu-cycles,instructions,cache-references,cache-misses \
    PracticaArcos2-release/imtool-soa/imtool-soa \
    PracticaArcos2/imagesPPM/lake-large.ppm \
    PracticaArcos2/imagesPPM/lake-large-soa-maxlevel-65535.ppm \
    maxlevel 65535

echo "Medición de rendimiento finalizada"

# Medir consumo de energía
perf stat -e power/energy-cores/,power/energy-pkg/,power/energy-ram/ \
    PracticaArcos2-release/imtool-soa/imtool-soa \
    PracticaArcos2/imagesPPM/lake-large.ppm \
    PracticaArcos2/imagesPPM/lake-large-soa-maxlevel-65535.ppm \
    maxlevel 65535

echo "Medición de energía finalizada"
echo "Fin Maxlevel"
# ------------------------------------------------
echo "Cutfreq lake-large 100000"
echo "Cutfreq AOS"
perf stat -e cpu-cycles,instructions,cache-references,cache-misses \
    PracticaArcos2-release/imtool-aos/imtool-aos \
    PracticaArcos2/imagesPPM/lake-large.ppm \
    PracticaArcos2/imagesPPM/lake-large-aos-cutfreq-100k.ppm \
    cutfreq 100000

echo "Medición de rendimiento finalizada"

# Medir consumo de energía
perf stat -e power/energy-cores/,power/energy-pkg/,power/energy-ram/ \
    PracticaArcos2-release/imtool-aos/imtool-aos \
    PracticaArcos2/imagesPPM/lake-large.ppm \
    PracticaArcos2/imagesPPM/lake-large-aos-cutfreq-100k.ppm \
    cutfreq 100000

echo "Medición de energía finalizada"

echo "Cutfreq SOA"
perf stat -e cpu-cycles,instructions,cache-references,cache-misses \
    PracticaArcos2-release/imtool-soa/imtool-soa \
    PracticaArcos2/imagesPPM/lake-large.ppm \
    PracticaArcos2/imagesPPM/lake-large-soa-cutfreq-100k.ppm \
    cutfreq 100000

echo "Medición de rendimiento finalizada"

# Medir consumo de energía
perf stat -e power/energy-cores/,power/energy-pkg/,power/energy-ram/ \
    PracticaArcos2-release/imtool-soa/imtool-soa \
    PracticaArcos2/imagesPPM/lake-large.ppm \
    PracticaArcos2/imagesPPM/lake-large-soa-cutfreq-100k.ppm \
    cutfreq 100000

echo "Medición de energía finalizada"
echo "Fin Cutfreq"
#-------------------------------------------------
echo "Compress lake-small"
echo "Compress AOS"
perf stat -e cpu-cycles,instructions,cache-references,cache-misses \
    PracticaArcos2-release/imtool-aos/imtool-aos \
    PracticaArcos2/imagesPPM/lake-small.ppm \
    PracticaArcos2/imagesPPM/lake-small-aos-compress.cppm \
    compress

echo "Medición de rendimiento finalizada"

# Medir consumo de energía
perf stat -e power/energy-cores/,power/energy-pkg/,power/energy-ram/ \
    PracticaArcos2-release/imtool-aos/imtool-aos \
    PracticaArcos2/imagesPPM/lake-small.ppm \
    PracticaArcos2/imagesPPM/lake-small-aos-compress.cppm \
    compress

echo "Medición de energía finalizada"

echo "Compress SOA"
perf stat -e cpu-cycles,instructions,cache-references,cache-misses \
    PracticaArcos2-release/imtool-soa/imtool-soa \
    PracticaArcos2/imagesPPM/lake-small.ppm \
    PracticaArcos2/imagesPPM/lake-small-soa-compress.cppm \
    compress

echo "Medición de rendimiento finalizada"

# Medir consumo de energía
perf stat -e power/energy-cores/,power/energy-pkg/,power/energy-ram/ \
    PracticaArcos2-release/imtool-soa/imtool-soa \
    PracticaArcos2/imagesPPM/lake-small.ppm \
    PracticaArcos2/imagesPPM/lake-small-soa-compress.cppm \
    compress

echo "Medición de energía finalizada"
echo "Fin Compress"