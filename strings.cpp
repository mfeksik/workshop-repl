#include <iostream>

#include "repl.hpp"

namespace repl
{
namespace strings
{

class Concat :
    public repl::BaseCommand<Concat, std::string, std::string, std::string>
{
public:
    const std::string_view longName = "concat";
    const std::string_view shortName = "ct";
    Concat() = default;

    std::string operator()(const std::string& lhs, const std::string& rhs) const
    {
        return lhs + rhs;
    }
};

class Find :
    public repl::BaseCommand<Find, std::size_t, std::string, char>
{
public:
    const std::string_view longName = "find";
    const std::string_view shortName = "fd";
    Find() = default;

    std::size_t operator()(const std::string& str, const char ch) const
    {
        const std::size_t pos = str.find(ch);
        if (pos == std::string::npos)
        {
            std::ostringstream msg{};
            msg << "character '" << ch << "' not found in '" << str << '\'';
            throw std::runtime_error(msg.str());
        }
        return pos;
    }
};

class Length :
    public repl::BaseCommand<Length, std::size_t, std::string>
{
public:
    const std::string_view longName = "length";
    const std::string_view shortName = "len";
    Length() = default;

    std::size_t operator()(const std::string& str) const
    {
        return str.size();
    }
};

class AtIndex :
    public repl::BaseCommand<AtIndex, char, std::string, std::size_t>
{
public:
    const std::string_view longName = "atindex";
    const std::string_view shortName = "at";
    AtIndex() = default;

    char operator()(const std::string& str, const std::size_t index) const
    {
        if (index >= str.size())
        {
            std::ostringstream msg{};
            msg << "index '" << index
                << "' is out of range for string of length '"
                << str.size() << '\'';
            throw std::out_of_range(msg.str());
        }
        return str.at(index);
    }
};

using Repl = ::repl::Repl<
    Concat,
    Find,
    Length,
    AtIndex
>;

} // namespace strings
} // namespace repl

int main()
{
    using namespace repl::strings;

    try
    {
        Repl repl{std::cin, std::cout, "str"};
        repl.Run();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& err)
    {
        std::cerr << "Error: " << err.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Error: unknown\n";
    }
    return EXIT_FAILURE;
}
