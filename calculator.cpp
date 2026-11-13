#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string_view>

#include "command.hpp"
#include "repl.hpp"

namespace repl
{
namespace calculator
{

class Multiply : public repl::BaseCommand<Multiply, float, float, float>
{
public:
    const std::string_view longName = "multiply";
    const std::string_view shortName = "mul";
    Multiply() = default;
    float operator()(const float lhs, const float rhs) const
    {
        return lhs * rhs;
    }
};

class SquareRoot : public repl::BaseCommand<SquareRoot, float, float>
{
public:
    const std::string_view longName = "squareroot";
    const std::string_view shortName = "sqrt";
    SquareRoot() = default;
    float operator()(const float number) const
    {
        return std::sqrt(number);
    }
};

using Complex = std::complex<float>;

class ComplexAdd
    : public repl::BaseCommand<ComplexAdd, Complex, Complex, Complex>
{
public:
    const std::string_view longName = "cadd";
    const std::string_view shortName = "ca";
    ComplexAdd() = default;
    Complex operator()(const Complex lhs, const Complex rhs) const
    {
        return lhs + rhs;
    }
};

class ComplexMultiply
    : public repl::BaseCommand<ComplexMultiply, Complex, Complex, Complex>
{
public:
    const std::string_view longName = "cmul";
    const std::string_view shortName = "cm";
    ComplexMultiply() = default;
    Complex operator()(const Complex lhs, const Complex rhs) const
    {
        return lhs * rhs;
    }
};

class ComplexAbs : public repl::BaseCommand<ComplexAbs, float, Complex>
{
public:
    const std::string_view longName = "cabs";
    const std::string_view shortName = "abs";
    ComplexAbs() = default;
    float operator()(const Complex number) const
    {
        return std::abs(number);
    }
};

using Repl = ::repl::Repl<
    Multiply,
    SquareRoot,
    ComplexAdd,
    ComplexMultiply,
    ComplexAbs
>;

} // namespace calculator
} // namespace repl

int main()
{
    using namespace repl::calculator;

    try
    {
        Repl repl{std::cin, std::cout, "calc"};
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
