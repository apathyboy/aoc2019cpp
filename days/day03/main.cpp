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

auto path_to_offset = [](auto&& s) {
    return std::stoi(s | rv::tail | rs::to<std::string>) * DIRECTIONS.at(rs::front(s));
};

auto next_segment(auto& prev)
{
    return [&prev](auto v) {
        line_segment l{prev.b, prev.b + v};
        prev = l;
        return l;
    };
}

auto read_input(std::istream& input)
{
    // clang-format off
    return rs::getlines(input) 
        | rv::transform([](auto&& s) {
            line_segment prev = {};
            return s | rv::split(',') 
                | rv::transform(path_to_offset)
                | rv::transform(next_segment(prev)) | rs ::to_vector; }) | rs::to_vector;
    // clang-format on
}

bool is_between(int a, int b, int check)
{
    return check <= std::max(a, b) && check >= std::min(a, b);
}

bool on_segment(const line_segment& ls, const glm::ivec2& p)
{
    return is_between(ls.a.x, ls.b.x, p.x) && is_between(ls.a.y, ls.b.y, p.y);
}

std::optional<glm::ivec2> intersect(const line_segment& l1, const line_segment& l2)
{
    if (is_between(l2.a.x, l2.b.x, l1.a.x) && is_between(l1.a.y, l1.b.y, l2.a.y)) {
        return glm::ivec2{l1.a.x, l2.a.y};
    }
    else if (is_between(l2.a.y, l2.b.y, l1.a.y) && is_between(l1.a.x, l1.b.x, l2.a.x)) {
        return glm::ivec2{l2.a.x, l1.a.y};
    }

    return std::nullopt;
}

int64_t manhattan(const glm::ivec2& a, const glm::ivec2& b)
{
    return std::abs(b.x - a.x) + std::abs(b.y - a.y);
}

int64_t path_distance(const std::vector<line_segment>& wire, const glm::ivec2& p)
{
    auto path    = wire | rv::take_while([&p](const auto& ls) { return !on_segment(ls, p); });
    auto lengths = path | rv::transform([](auto&& ls) { return manhattan(ls.a, ls.b); });

    return rs::accumulate(lengths, int64_t{0}) + manhattan((path | rv::reverse).begin()->b, p);
}

int64_t part1(std::vector<line_segment> wire1, std::vector<line_segment> wire2)
{
    // clang-format off
    auto intersections = rv::cartesian_product(wire1, wire2) 
        | rv::transform([](auto&& p) {
            return intersect(std::get<0>(p), std::get<1>(p)); })
        | rv::filter([](const auto& p) {
            return p.has_value() && p.value() != glm::ivec2{0, 0}; })
        | rv::transform([](auto&& p) { 
            glm::ivec2 origin{0, 0}; 
            return manhattan(origin,p.value()); });
    // clang-format on

    return rs::min(intersections);
}

int64_t part2(std::vector<line_segment> wire1, std::vector<line_segment> wire2)
{
    // clang-format off
    auto intersections = rv::cartesian_product(wire1, wire2) 
        | rv::transform([](auto&& p) {
            return intersect(std::get<0>(p), std::get<1>(p)); })
        | rv::filter([](const auto& p) {
            return p.has_value() && p.value() != glm::ivec2{0, 0}; })
        | rv::transform([&wire1, &wire2](auto&& p) {
            return path_distance(wire1, p.value()) + path_distance(wire2, p.value()); });
    // clang-format on

    return rs::min(intersections);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2019 - Day 03\n");

    std::ifstream ifs{"days/day03/puzzle.in"};

    auto input = read_input(ifs);

    fmt::print("Part 1 Solution: {}\n", part1(input[0], input[1]));
    fmt::print("Part 2 Solution: {}\n", part2(input[0], input[1]));

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

    ss << R"(R8,U5,L5,D3
U7,R6,D4,L4)";

    auto input = read_input(ss);

    REQUIRE(30 == part2(input[0], input[1]));
}

#endif
