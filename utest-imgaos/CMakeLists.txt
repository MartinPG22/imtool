add_executable(utest-imgaos
        utest_resize_aos.cpp
        )

# Vincular Google Test
target_link_libraries(utest-imgaos PRIVATE imgaos GTest::gtest GTest::gtest_main)

