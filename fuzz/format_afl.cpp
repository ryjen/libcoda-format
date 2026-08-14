#include <coda/format/format.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>

namespace
{
    constexpr std::size_t max_input_size = 4096;
    constexpr std::size_t max_arguments = 64;

    void exercise(coda::format &value)
    {
        const auto count = std::min(value.specifiers(), max_arguments);
        for (std::size_t index = 0; index < count; ++index) {
            value.args(123.125);
        }

        const auto rendered = value.str();
        (void)rendered;
    }
}

int main()
{
    std::string input((std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>());
    if (input.size() > max_input_size) {
        input.resize(max_input_size);
    }

    try {
        coda::format parsed(input);

        coda::format copied(parsed);
        coda::format moved(std::move(copied));
        exercise(moved);

        moved.reset(input);
        exercise(moved);
    } catch (const std::invalid_argument &) {
        // Invalid format strings are expected parser outcomes, not fuzz crashes.
    }

    return 0;
}
