#include <fmt/format.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;

[[nodiscard]] auto generate_input(std::istream& input)
{
    return rs::getlines(input)
           | rv::transform([](auto&& s) { return std::stoi(s | rs::to<std::string>); })
           | rs::to_vector;
}

[[nodiscard]] int calculate_fuel(int mass) noexcept
{
    return (mass / 3) - 2;
}

[[nodiscard]] int calculate_total_fuel(int module_mass) noexcept
{
    return rs::accumulate(
        rv::iota(0)
            | rv::exclusive_scan(module_mass, [](auto i, auto) { return calculate_fuel(i); })
            | rv::tail | rv::take_while([](int i) { return i > 0; }),
        0);
}

int64_t part1(std::vector<int> input)
{
    return rs::accumulate(input | rv::transform(calculate_fuel), 0);
}

int64_t part2(std::vector<int> input)
{
    return rs::accumulate(input | rv::transform(calculate_total_fuel), 0);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2019 - Day 01\n");

    std::ifstream ifs{"days/day01/puzzle.in"};

    auto input = generate_input(ifs);

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can calculate fuel for mass")
{
    REQUIRE(2 == calculate_fuel(12));
    REQUIRE(654 == calculate_fuel(1969));
}

TEST_CASE("Can calculate total fuel for mass")
{
    REQUIRE(2 == calculate_total_fuel(14));
    REQUIRE(966 == calculate_total_fuel(1969));
    REQUIRE(50346 == calculate_total_fuel(100756));
}

#endif
