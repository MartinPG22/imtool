# Add to this list all files related to imgaos library
add_library(imgaos
        maxlevel.cpp
        maxlevel.hpp
        imageaos.cpp
        imageaos.hpp
        resize.cpp
        resize.hpp
        compress.cpp
        compress.hpp
        cutfreq.cpp
        cutfreq.hpp
)
# Use this line only if you have dependencies from this library to GSL
# find_package(GSL REQUIRED)
# target_link_libraries (common PRIVATE Microsoft.GSL::GSL) ty