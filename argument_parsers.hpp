#pragma once

#include <complex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

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
            std::size_t pos = 0;
            const float val = std::stof(std::string{str}, &pos);
            if (pos != str.size())
                throw std::invalid_argument("");

            return val;
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
struct ArgParser<std::complex<float>>
{
    static std::complex<float> parse(std::string_view str)
    {
        if (str.empty())
            throw_invalid(str);

        if (str.back() != 'i')
            return { parse_part(str, str), 0.0f };

        const std::string_view body = str.substr(0, str.size() - 1);
        if (body.empty())
            throw_invalid(str);

        std::size_t split_pos = std::string_view::npos;
        for (std::size_t j = body.size(); j > 1; --j)
        {
            const std::size_t idx = j - 1;
            const char c = body[idx];
            if (c == '+' || c == '-')
            {
                const char prev = body[idx - 1];
                if (prev != 'e' && prev != 'E')
                {
                    split_pos = idx;
                    break;
                }
            }
        }

        if (split_pos == std::string_view::npos)
            return { 0.0f, parse_part(body, str) };

        const std::string_view real_part = body.substr(0, split_pos);
        const std::string_view img_part = body.substr(split_pos);

        return {parse_part(real_part, str), parse_part(img_part, str)};
    }

private:
    static float parse_part(std::string_view part, std::string_view full_str)
    {
        if (part.empty())
            throw_invalid(full_str);

        if (part.size() > 1
            && (part[0] == '+' || part[0] == '-')
            && (part[1] == '+' || part[1] == '-'))
        {
            throw_invalid(full_str);
        }

        try
        {
            return ArgParser<float>::parse(part);
        }
        catch (const std::invalid_argument&)
        {
            throw_invalid(full_str);
        }
        catch (const std::out_of_range&)
        {
            throw_out_of_range(full_str);
        }
    }

    [[noreturn]] static void throw_invalid(std::string_view full_str)
    {
        std::ostringstream msg{};
        msg << "failed to parse complex from '" << full_str << '\'';
        throw std::invalid_argument(msg.str());
    }

    [[noreturn]] static void throw_out_of_range(std::string_view full_str)
    {
        std::ostringstream msg{};
        msg << "value is out of range for complex: '" << full_str << '\'';
        throw std::out_of_range(msg.str());
    }
};

} // namespace repl
