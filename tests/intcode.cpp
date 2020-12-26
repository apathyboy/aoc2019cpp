
#include <aoc2019/intcode.hpp>

#include <catch2/catch.hpp>

#include <sstream>

TEST_CASE("Can read intcode input programs")
{
    std::stringstream ss;

    ss << R"(1,9,10,3,2,3,11,0,99,30,40,50)";

    auto program = aoc::read_intcode_program(ss);

    REQUIRE(12 == program.size());
    REQUIRE(1 == program[0]);
    REQUIRE(3 == program[5]);
    REQUIRE(50 == program[11]);
}