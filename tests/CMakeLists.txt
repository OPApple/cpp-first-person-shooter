add_executable(data_tests data_tests.cpp)

target_include_directories(data_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(data_tests PRIVATE
    test_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(data_tests)
