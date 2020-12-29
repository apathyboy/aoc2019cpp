#include <fmt/format.h>
#include <glm/vec2.hpp>
#include <glm/gtx/io.hpp>
#include <range/v3/all.hpp>

#include <fstream>
#include <map>
#include <optional>

namespace rs = ranges;
namespace rv = ranges::views;

struct line_segment {
    glm::ivec2 a;
    glm::ivec2 b;
};

const std::map<char, glm::ivec2> DIRECTIONS{
    {'R', {1, 0}},
    {'L', {-1, 0}},
    {'U', {0, 1}},
    {'D', {0, -1}}};

auto read_input(std::istream& input)
{
    return rs::getlines(input) | rv::transform([](auto&& s) {
               line_segment prev = {};
               return s | rv::split(',') | rv::transform([](auto&& s) {
                          char dir = rs::front(s);
                          int  amt = std::stoi(s | rv::tail | rs::to<std::string>);
                          return amt * DIRECTIONS.at(dir);
                      })
                      | rv::transform([&prev](auto v) {
                            line_segment l{prev.b, prev.b + v};
                            prev = l;
                            return l;
                        })
                      | rs ::to_vector;
           })
           | rs::to_vector;
}


std::optional<glm::ivec2>
calculate_intersection(const line_segment& l1, const line_segment& l2)
{
    std::optional<glm::ivec2> intersection;

    // line1 is vertical
    if (l1.a.x == l1.b.x) {
        // is line1 x between x of line2 endpoints?
        if (l1.a.x <= std::max(l2.a.x, l2.b.x) && l1.a.x >= std::min(l2.a.x, l2.b.x)) {
            // and is line2 y between y of line1 endpoints?
            if (l2.a.y <= std::max(l1.a.y, l1.b.y) && l2.a.y >= std::min(l1.a.y, l1.b.y)) {
                // then intersected at line1.a.x, line2.a.y
                intersection = {l1.a.x, l2.a.y};
            }
        }
    }
    // else it is horizontal
    else {
        // is line1 y between y of line2 endpoints
        if (l1.a.y <= std::max(l2.a.y, l2.b.y) && l1.a.y >= std::min(l2.a.y, l2.b.y)) {
            // and is line2 x between x of line1 endpoints
            if (l2.a.x <= std::max(l1.a.x, l1.b.x) && l2.a.x >= std::min(l1.a.x, l1.b.x)) {
                // then intersected at line2.a.x, line1.a.y
                intersection = {l2.a.x, l1.a.y};
            }
        }
    }

    return intersection;
}

int64_t manhattan(const glm::ivec2& p)
{
    return p.x + p.y;
}

int64_t part1(std::vector<line_segment> wire1, std::vector<line_segment> wire2)
{
    auto intersections = rv::cartesian_product(wire1, wire2) | rv::transform([](auto&& p) {
                             const auto& [s1, s2] = p;
                             return calculate_intersection(s1, s2);
                         })
                         | rv::filter([](const auto& p) {
                               return p.has_value() && p.value() != glm::ivec2{0, 0};
                           })
                         | rv::transform([](auto&& p) { return manhattan(p.value()); });

    return rs::min(intersections);
}

int64_t part2()
{
    return 0;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2019 - Day 03\n");

    std::ifstream ifs{"days/day03/puzzle.in"};

    auto input = read_input(ifs);

    fmt::print("Part 1 Solution: {}\n", part1(input[0], input[1]));
    fmt::print("Part 2 Solution: {}\n", part2());

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can read wire input")
{
    std::stringstream ss;

    ss << R"(R8,U5,L5,D3
U7,R6,D4,L4)";

    auto input = read_input(ss);

    REQUIRE(2 == input.size());
    REQUIRE(4 == input[0].size());
    REQUIRE(4 == input[1].size());
}

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(R8,U5,L5,D3
U7,R6,D4,L4)";

    auto input = read_input(ss);

    REQUIRE(6 == part1(input[0], input[1]));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"()";

    REQUIRE(0 == part2());
}

#endif
