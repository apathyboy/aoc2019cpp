#include <aoc2019/intcode.hpp>

#include <fmt/format.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;

int64_t compute(std::vector<int> program)
{
    size_t ip = 0;

    while (program[ip] != 99) {
        switch (program[ip]) {
            case 1: {
                program[program[ip + 3]] = program[program[ip + 1]] + program[program[ip + 2]];
                ip += 4;
            } break;
            case 2: {
                program[program[ip + 3]] = program[program[ip + 1]] * program[program[ip + 2]];
                ip += 4;
            } break;
            default: {
                std::runtime_error{"Invalid opcode " + program[ip]};
            } break;
        }
    }

    return program[0];
}

int64_t compute(std::vector<int> program, int noun, int verb)
{
    program[1] = noun, program[2] = verb;

    return compute(std::move(program));
}

auto compute_with_noun_and_verb(const std::vector<int>& program)
{
    return [&program](auto&& p) {
        auto [noun, verb] = p;
        return std::make_tuple(noun, verb, compute(program, noun, verb));
    };
}

auto keep_output_match(int keep_result)
{
    return [keep_result](const auto& t) {
        auto [_, __, result] = t;
        return result == keep_result;
    };
}

int64_t part1(std::vector<int> program)
{
    return compute(program, 12, 2);
}

int64_t part2(std::vector<int> program)
{
    auto matches = rv::cartesian_product(rv::iota(0, 99), rv::iota(0, 99))
                   | rv::transform(compute_with_noun_and_verb(program))
                   | rv::filter(keep_output_match(19690720));

    auto [noun, verb, _] = rs::front(matches);

    return (100 * noun) + verb;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2019 - Day 02\n");

    std::ifstream ifs{"days/day02/puzzle.in"};

    auto program = aoc::read_intcode_program(ifs);

    fmt::print("Part 1 Solution: {}\n", part1(program));
    fmt::print("Part 2 Solution: {}\n", part2(program));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can run simple intcode program")
{
    REQUIRE(3500 == compute(std::vector{1, 9, 10, 3, 2, 3, 11, 0, 99, 30, 40, 50}));
    REQUIRE(30 == compute(std::vector{1, 1, 1, 4, 99, 5, 6, 0, 99}));
}

#endif
