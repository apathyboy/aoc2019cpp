
#include "aoc2019.hpp"

#include <range/v3/all.hpp>

namespace rs = ranges;
namespace rv = ranges::views;

namespace aoc {

std::vector<int> read_intcode_program(std::istream& input)
{
    return rs::getlines(input, ',')
           | rv::transform([](auto&& s) { return std::stoi(s | rs::to<std::string>); })
           | rs::to_vector;
}

} // namespace aoc
