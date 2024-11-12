add_executable(utest-imgsoa
        utest_compress_soa.cpp
        utest_resize_soa.cpp
        utest_maxlevel_soa.cpp
        utest_imagesoa.cpp
        utest_cutfreq_soa.cpp

)

# Vincular Google Test
target_link_libraries(utest-imgsoa PRIVATE imgsoa GTest::gtest GTest::gtest_main)