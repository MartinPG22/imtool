add_executable(utest-imgaos
        utest_resize_aos.cpp
        utest_cutfreq_aos.cpp
        utest_aos.hpp
        utest_compress_aos.cpp
        utest_maxlevel_aos.cpp
        utest_imageaos.cpp
)

# Vincular Google Test
target_link_libraries(utest-imgaos PRIVATE imgaos GTest::gtest GTest::gtest_main)

