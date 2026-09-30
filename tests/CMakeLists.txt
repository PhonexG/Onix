find_package(Catch2 3 REQUIRED)

add_executable(onix_tests
    test_main.cpp
)

target_link_libraries(onix_tests
    Catch2::Catch2WithMain
    onix_core
    onix_compile_options
)

include(CTest)
include(Catch)
catch_discover_tests(onix_tests)
