# Add to this list all files related to common library
add_library(common
        progargs.cpp
        binaryio.cpp
        progargs.hpp
        binaryio.hpp
        ../utest-imagsoa/utest_soa.hpp
)
# Link imgsoa with common
target_link_libraries(common PUBLIC imgsoa)

# Use this line only if you have dependencies from this library to GSL
#find_package(GSL REQUIRED)
#target_link_libraries (common PRIVATE Microsoft.GSL::GSL)