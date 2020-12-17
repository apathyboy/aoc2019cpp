add_executable(aoc2019-tests main.cpp)

target_link_libraries(
    aoc2019-tests
    PRIVATE Catch2::Catch2
            esb::aoc2019)

target_compile_options(aoc2019-tests PRIVATE -wd6330)

catch_discover_tests(aoc2019-tests)
