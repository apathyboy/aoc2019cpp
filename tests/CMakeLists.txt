add_executable(aoc2019-tests main.cpp)

target_link_libraries(
    aoc2019-tests
    PRIVATE Catch2::Catch2
            esb::aoc2019)

target_compile_options(
    aoc2019-tests
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            -wd4201
            -wd4996
            -wd4459 # TODO range-v3 error
            -wd4702 # TODO range-v3 error
            -wd6330>)

catch_discover_tests(aoc2019-tests)
