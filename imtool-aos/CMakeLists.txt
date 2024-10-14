add_executable(imtool-aos main.cpp)
target_link_libraries (imtool-aos imgaos common)

# Use this line only if you have dependencies from this library to GSL
find_package(GSL REQUIRED)
target_link_libraries (common PRIVATE Microsoft.GSL::GSL)