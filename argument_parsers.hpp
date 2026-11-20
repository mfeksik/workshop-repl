#pragma once

#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <charconv>

namespace repl
{

template<class T>
struct ArgParser;

template<>
struct ArgParser<float>
{
    static float parse(std::string_view str)
    {
        try
        {
            return std::stof(std::string{str});
        }
        catch (const std::invalid_argument& err)
        {
            // std::stof message is kind of stupid,
            // we will replace it
            std::ostringstream msg{};
            msg << "failed to parse float from '" << str << '\'';
            throw std::invalid_argument(msg.str());
        }
        catch (const std::out_of_range& err)
        {
            std::ostringstream msg{};
            msg << "value is out of range for float: '" << str << '\'';
            throw std::out_of_range(msg.str());
        }
    }
};

template<>
struct ArgParser<std::string>
{
    static std::string parse(std::string_view sv)
    {
        return std::string(sv);
    }
};

template<>
struct ArgParser<char>
{
    static char parse(std::string_view sv)
    {
        if (sv.size() != 1)
        {
            std::ostringstream msg{};
            msg << "failed to parse char from '" << sv << '\'';
            throw std::invalid_argument(msg.str());
        }

        return sv.front();
    }
};

template<>
struct ArgParser<std::size_t>
{
    static std::size_t parse(std::string_view sv)
    {
        std::size_t val = 0;
        const auto [ptr, ec] = std::from_chars(
            sv.data(),
            sv.data() + sv.size(),
            val
        );
        if (ec != std::errc{} || ptr != sv.data() + sv.size())
            throw std::invalid_argument(
                "failed to parse size_t from '" + std::string(sv) + '\'');

        return val;
    }
};

} // namespace repl
