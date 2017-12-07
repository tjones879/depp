#include "inc/builtin.hpp"
#include "inc/ast.h"
#include <vector>
#include <string>
#include <algorithm>
#include <type_traits>
#include <numeric>
#include <iostream>

namespace depp {

class TypeMismatchException : public std::runtime_error {
public:
    TypeMismatchException() : runtime_error("There were different types.") {}
    TypeMismatchException(std::string msg) : runtime_error(msg.c_str()) {}
};

class ArgLengthException : public std::runtime_error {
public:
    ArgLengthException() : runtime_error("There were different types.") {}
    ArgLengthException(std::string msg) : runtime_error(msg.c_str()) {}
};

template <class F, class G>
auto compose(F f, G g) {
    return [f, g](auto x){ return f(g(x)); };
}

template <class F, class G>
auto operator*(F f, G g) {
    return compose(f, g);
}

template <class F, typename... Fs>
auto compose(F f, Fs&&... fs) {
    return f * compose(fs...);
}

template <class T, class C>
auto foldLiterals(const std::vector<T> &deps, C composition) {

}

ast::LiteralVariant addInt(ast::LiteralVariant &sum, int term) {
    if (std::holds_alternative<int>(sum)) {
        auto rep = std::get<int>(sum);
        return ast::LiteralVariant(rep + term);
    } else if (std::holds_alternative<double>(sum)) {
        auto rep = std::get<double>(sum);
        return ast::LiteralVariant(rep + term);
    } else {
        throw TypeMismatchException();
    }
}

ast::LiteralVariant subInt(ast::LiteralVariant &diff, int term) {
    if (std::holds_alternative<int>(diff)) {
        auto rep = std::get<int>(diff);
        return ast::LiteralVariant(rep - term);
    } else if (std::holds_alternative<double>(diff)) {
        auto rep = std::get<double>(diff);
        return ast::LiteralVariant(rep - term);
    } else {
        throw TypeMismatchException();
    }
}

ast::LiteralVariant addDouble(ast::LiteralVariant &sum, double term) {
    double rep;
    if (std::holds_alternative<int>(sum)) {
        rep = std::get<int>(sum);
    } else if (std::holds_alternative<double>(sum)) {
        rep = std::get<double>(sum);
    } else {
        throw TypeMismatchException();
    }
    return ast::LiteralVariant(rep + term);
}

ast::LiteralVariant subDouble(ast::LiteralVariant &diff, double term) {
    double rep;
    if (std::holds_alternative<int>(diff)) {
        rep = std::get<int>(diff);
    } else if (std::holds_alternative<double>(diff)) {
        rep = std::get<double>(diff);
    } else {
        throw TypeMismatchException();
    }
    return ast::LiteralVariant(rep - term);
}

ast::LiteralVariant multiply(ast::LiteralVariant &prod, int term) {
    if (std::holds_alternative<int>(prod)) {
        auto rep = std::get<int>(prod);
        return ast::LiteralVariant(rep * term);
    } else if (std::holds_alternative<double>(prod)) {
        auto rep = std::get<double>(prod);
        return ast::LiteralVariant(rep * term);
    } else {
        throw TypeMismatchException();
    }
}

ast::LiteralVariant multiply(ast::LiteralVariant &prod, double term) {
    double rep;
    if (std::holds_alternative<int>(prod)) {
        rep = std::get<int>(prod);
    } else if (std::holds_alternative<double>(prod)) {
        rep = std::get<double>(prod);
    } else {
        throw TypeMismatchException();
    }
    return ast::LiteralVariant(rep * term);
}

ast::LiteralVariant divide(ast::LiteralVariant &diff, int term) {
    if (std::holds_alternative<int>(diff)) {
        auto rep = std::get<int>(diff);
        return ast::LiteralVariant(rep / term);
    } else if (std::holds_alternative<double>(diff)) {
        auto rep = std::get<double>(diff);
        return ast::LiteralVariant(rep / term);
    } else {
        throw TypeMismatchException();
    }
}

ast::LiteralVariant divide(ast::LiteralVariant &div, double term) {
    double rep;
    if (std::holds_alternative<int>(div)) {
        rep = std::get<int>(div);
    } else if (std::holds_alternative<double>(div)) {
        rep = std::get<double>(div);
    } else {
        throw TypeMismatchException();
    }
    return ast::LiteralVariant(rep / term);
}

ast::LiteralNode proc_add(std::vector<ast::LiteralNode> &deps)
{
    ast::LiteralVariant sum(0);
    bool empty = true;
    std::for_each(deps.begin(), deps.end(),
        [&] (ast::LiteralNode &in) {
            std::visit([&](auto &arg) {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, int>) {
                    empty = false;
                    sum = addInt(sum, arg);
                } else if constexpr(std::is_same_v<T, double>) {
                    empty = false;
                    sum = addDouble(sum, arg);
                }
                else if constexpr(std::is_same_v<T, std::string>) {
                    if (empty) {
                        sum = ast::LiteralVariant(arg);
                        empty = false;
                    } else {
                        if (!std::holds_alternative<std::string>(sum)) {
                            throw TypeMismatchException();
                        } else {
                            auto str = std::get<std::string>(sum);
                            sum = ast::LiteralVariant(str + arg);
                        }
                    }
                }
            }, in.literal);
        });

    // We want to return an integer type if possible, otherwise return double
    if (std::holds_alternative<int>(sum)) {
        return ast::LiteralNode(ast::LiteralType::INTEGER, sum);
    } else if (std::holds_alternative<double>(sum)) {
        return ast::LiteralNode(ast::LiteralType::REAL, sum);
    } else if (std::holds_alternative<std::string>(sum)) {
        return ast::LiteralNode(ast::LiteralType::STRING, sum);
    } else {
        throw TypeMismatchException();
    }
}

ast::LiteralNode proc_sub(std::vector<ast::LiteralNode> &deps)
{
    if (deps.size() < 2)
        throw ArgLengthException();

    ast::LiteralVariant diff(deps.begin()->literal);
    std::for_each(deps.begin() + 1, deps.end(),
        [&] (ast::LiteralNode &in) {
            std::visit([&](auto &arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>) {
                    diff = subInt(diff, arg);
                } else if constexpr (std::is_same_v<T, double>) {
                    diff = subDouble(diff, arg);
                } else {
                    throw TypeMismatchException();
                }
            }, in.literal);
        });

    if (std::holds_alternative<int>(diff)) {
        return ast::LiteralNode(ast::LiteralType::INTEGER, diff);
    } else if (std::holds_alternative<double>(diff)) {
        return ast::LiteralNode(ast::LiteralType::REAL, diff);
    } else {
        throw TypeMismatchException();
    }
}

ast::LiteralNode proc_mult(std::vector<ast::LiteralNode> &deps)
{
    ast::LiteralVariant prod(1);

    std::for_each(deps.begin(), deps.end(),
        [&] (ast::LiteralNode &in) {
            std::visit([&](auto &arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>) {
                    prod = multiply(prod, arg);
                } else if constexpr (std::is_same_v<T, double>) {
                    prod = multiply(prod, arg);
                } else {
                    throw TypeMismatchException();
                }
            }, in.literal);
        });

    if (std::holds_alternative<int>(prod)) {
        return ast::LiteralNode(ast::LiteralType::INTEGER, prod);
    } else if (std::holds_alternative<double>(prod)) {
        return ast::LiteralNode(ast::LiteralType::REAL, prod);
    } else {
        throw TypeMismatchException();
    }
}

ast::LiteralNode proc_div(std::vector<ast::LiteralNode> &deps)
{
    if (deps.size() < 2)
        throw ArgLengthException();

    ast::LiteralVariant div(deps.begin()->literal);
    std::for_each(deps.begin() + 1, deps.end(),
        [&] (ast::LiteralNode &in) {
            std::visit([&](auto &arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>) {
                    div = divide(div, arg);
                } else if constexpr (std::is_same_v<T, double>) {
                    div = divide(div, arg);
                } else {
                    throw TypeMismatchException();
                }
            }, in.literal);
        });

    if (std::holds_alternative<int>(div)) {
        return ast::LiteralNode(ast::LiteralType::INTEGER, div);
    } else if (std::holds_alternative<double>(div)) {
        return ast::LiteralNode(ast::LiteralType::REAL, div);
    } else {
        throw TypeMismatchException();
    }
}
}; // namespace depp
