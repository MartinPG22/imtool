add_executable(imtool-aos main.cpp
        main.hpp)
target_link_libraries(imtool-aos imgsoa common imgaos)

# Use this line only if you have dependencies from this library to GSL
#find_package(GSL REQUIRED)
#target_link_libraries (common PRIVATE Microsoft.GSL::GSL)