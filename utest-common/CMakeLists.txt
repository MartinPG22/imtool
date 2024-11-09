add_executable(utest-common
        utest_binaryio.cpp
        utest_common.hpp
        utest_progargs.cpp
)

# Vincular Google Test
target_link_libraries(utest-common PRIVATE common imgaos imgsoa GTest::gtest_main)