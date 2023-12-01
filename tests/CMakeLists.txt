find_package(Catch2 REQUIRED)

add_executable(tests
    test_csv.cpp
    test_warehouse.cpp
)
target_link_libraries(tests Catch2::Catch2WithMain aco)

include(Catch)
catch_discover_tests(tests)