# Add to this list all files related to imgaos library
add_library(imgaos
        other1.cpp
        other2.cpp
        imageaos.cpp
        imageaos.hpp
)
# Use this line only if you have dependencies from this library to GSL
# find_package(GSL REQUIRED)
# target_link_libraries (common PRIVATE Microsoft.GSL::GSL)