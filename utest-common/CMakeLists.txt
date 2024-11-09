add_executable(utest-common
        utest_binaryio.cpp
        utest_common.hpp

)

# Vincular Google Test
target_link_libraries(utest-common PRIVATE common GTest::gtest GTest::gtest_main)