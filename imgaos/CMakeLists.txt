# Add to this list all files related to imgaos library
add_library(imgaos
        other1.cpp
        other2.cpp
        maxlevel.cpp
        maxlevel.h
        imageaos.cpp
        imageaos.hpp
        resize.cpp
        resize.hpp
)
# Use this line only if you have dependencies from this library to GSL
# find_package(GSL REQUIRED)
# target_link_libraries (common PRIVATE Microsoft.GSL::GSL)