#!/bin/bash

exec > >(tee -a ftest-imtool-aos.txt) 2>&1

base_dir="$(dirname "$(realpath "${BASH_SOURCE[0]}")")"
imtool_executable="$base_dir/../cmake-build-debug/imtool-aos/imtool-aos"

deer_small="$base_dir/../imagesPPM/input/deer-small.ppm"
lake_large="$base_dir/../imagesPPM/input/lake-large.ppm"
lake_small="$base_dir/../imagesPPM/input/lake-small.ppm"
sabatini="$base_dir/../imagesPPM/input/sabatini.ppm"

output_dir="$base_dir/../imagesPPM"
deer_small_output="deer-small-output-aos.ppm"
lake_large_output="lake-large-output-aos.ppm"
lake_small_output="lake-small-output-aos.ppm"
sabatini_output="sabatini-output-aos.ppm"

# Prueba de argumentos insuficientes
echo -e "\nPRUEBA DE ARGUMENTOS INSUFICIENTES"
"$imtool_executable"
"$imtool_executable" input.ppm
"$imtool_executable" input.ppm output.ppm

# Prueba de operación inválida
echo -e "\nPRUEBA DE OPERACIÓN INVÁLIDA"
"$imtool_executable" input.ppm output.ppm invalid_op

# Prueba de operación info
echo -e "\nPRUEBA DE OPERACIÓN INFO"
"$imtool_executable" "$deer_small" "$output_dir/info_output/$deer_small_output" info
echo "---------------------------------------------------------------------------------------"
"$imtool_executable" "$lake_large" "$output_dir/info_output/$lake_large_output" info
echo "---------------------------------------------------------------------------------------"
"$imtool_executable" "$lake_small" "$output_dir/info_output/$lake_small_output" info
echo "---------------------------------------------------------------------------------------"
"$imtool_executable" "$sabatini" "$output_dir/info_output/$sabatini_output" info
echo "---------------------------------------------------------------------------------------"
echo "Info con argumentos extra"
"$imtool_executable" "$deer_small" "$output_dir/info_output/$deer_small_output" info 100
echo "---------------------------------------------------------------------------------------"

echo -e "\nPRUEBA DE OPERACIÓN MAXLEVEL"
"$imtool_executable" "$deer_small" "$output_dir/maxlevel_output/$deer_small_output" maxlevel 128
echo "---------------------------------------------------------------------------------------"
"$imtool_executable" "$lake_large" "$output_dir/maxlevel_output/$lake_large_output" maxlevel 128
echo "---------------------------------------------------------------------------------------"
"$imtool_executable" "$lake_small" "$output_dir/maxlevel_output/$lake_small_output" maxlevel 128
echo "---------------------------------------------------------------------------------------"
"$imtool_executable" "$sabatini" "$output_dir/maxlevel_output/$sabatini_output" maxlevel 128
echo "---------------------------------------------------------------------------------------"
echo "Maxlevel con argumentos extra"
"$imtool_executable" "$deer_small" "$output_dir/maxlevel_output/$deer_small_output" maxlevel 128 200
"$imtool_executable" "$deer_small" "$output_dir/maxlevel_output/$deer_small_output" maxlevel
echo "---------------------------------------------------------------------------------------"
echo "Maxlevel con valor negativo"
"$imtool_executable" "$deer_small" "$output_dir/maxlevel_output/$deer_small_output" maxlevel -1
echo "---------------------------------------------------------------------------------------"
echo "Maxlevel con valor mayor al indicado"
"$imtool_executable" "$deer_small" "$output_dir/maxlevel_output/$deer_small_output" maxlevel 70000
echo "---------------------------------------------------------------------------------------"
echo "Maxlevel con valor no numérico"
"$imtool_executable" "$deer_small" "$output_dir/maxlevel_output/$deer_small_output" maxlevel copy
echo "---------------------------------------------------------------------------------------"

echo -e "\nPRUEBA DE OPERACIÓN RESIZE"
"$imtool_executable" "$deer_small" "$output_dir/resize_output/$deer_small_output" resize 200 150
echo "---------------------------------------------------------------------------------------"
"$imtool_executable" "$lake_large" "$output_dir/resize_output/$lake_large_output" resize 200 150
echo "---------------------------------------------------------------------------------------"
"$imtool_executable" "$lake_small" "$output_dir/resize_output/$lake_small_output" resize 200 150
echo "---------------------------------------------------------------------------------------"
"$imtool_executable" "$sabatini" "$output_dir/resize_output/$sabatini_output" resize 200 150
echo "---------------------------------------------------------------------------------------"
echo "Resize con argumentos extra"
"$imtool_executable" "$deer_small" "$output_dir/resize_output/$deer_small_output" resize 200 150 100
"$imtool_executable" "$deer_small" "$output_dir/resize_output/$deer_small_output" resize 200
"$imtool_executable" "$deer_small" "$output_dir/resize_output/$deer_small_output" resize
echo "---------------------------------------------------------------------------------------"
echo "Resize con valor negativo"
"$imtool_executable" "$deer_small" "$output_dir/resize_output/$deer_small_output" resize -100 200
"$imtool_executable" "$deer_small" "$output_dir/resize_output/$deer_small_output" resize 100 -200
echo "---------------------------------------------------------------------------------------"
echo "Resize con valor no numérico"
"$imtool_executable" "$deer_small" "$output_dir/resize_output/$deer_small_output" resize 100 max
echo "---------------------------------------------------------------------------------------"

echo -e "\nPRUEBA DE OPERACIÓN CUTFREQ"
"$imtool_executable" "$deer_small" "$output_dir/cutfreq_output/$deer_small_output" cutfreq 5
echo "---------------------------------------------------------------------------------------"
"$imtool_executable" "$lake_large" "$output_dir/cutfreq_output/$lake_large_output" cutfreq 5
echo "---------------------------------------------------------------------------------------"
"$imtool_executable" "$lake_small" "$output_dir/cutfreq_output/$lake_small_output" cutfreq 5
echo "---------------------------------------------------------------------------------------"
"$imtool_executable" "$sabatini" "$output_dir/cutfreq_output/$sabatini_output" cutfreq 5
echo "---------------------------------------------------------------------------------------"
echo "Cutfreq con argumentos extra"
"$imtool_executable" "$deer_small" "$output_dir/cutfreq_output/$deer_small_output" cutfreq 5 10
"$imtool_executable" "$deer_small" "$output_dir/cutfreq_output/$deer_small_output" cutfreq
echo "---------------------------------------------------------------------------------------"
echo "Cutfreq con valor negativo"
"$imtool_executable" "$deer_small" "$output_dir/cutfreq_output/$deer_small_output" cutfreq -1
echo "---------------------------------------------------------------------------------------"
echo "Cutfreq con valor no numérico"
"$imtool_executable" "$deer_small" "$output_dir/cutfreq_output/$deer_small_output" cutfreq copy

# Prueba de operación compress
echo -e "\nPRUEBA DE OPERACIÓN COMPRESS"
"$imtool_executable" "$deer_small" "$output_dir/compress_output/$deer_small_output" compress
echo "---------------------------------------------------------------------------------------"
"$imtool_executable" "$lake_large" "$output_dir/compress_output/$lake_large_output" compress
echo "---------------------------------------------------------------------------------------"
"$imtool_executable" "$lake_small" "$output_dir/compress_output/$lake_small_output" compress
echo "---------------------------------------------------------------------------------------"
"$imtool_executable" "$sabatini" "$output_dir/compress_output/$sabatini_output" compress
echo "---------------------------------------------------------------------------------------"
echo "Compress con argumentos extra"
"$imtool_executable" "$deer_small" "$output_dir/compress_output/$deer_small_output" compress 100
