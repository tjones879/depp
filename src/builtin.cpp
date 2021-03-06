#include "inc/builtin.hpp"
#include "inc/ast.h"
#include "inc/env.hpp"
#include "inc/utils.hpp"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace depp {

template <class F, class G>
auto compose(F f, G g)
{
    return [f, g](auto x) { return f(g(x)); };
}

template <class F, class G>
auto operator*(F f, G g)
{
    return compose(f, g);
}

template <class F, typename... Fs>
auto compose(F f, Fs &&... fs)
{
    return f * compose(fs...);
}

template <class T, class C>
auto foldLiterals(const std::vector<ast::LiteralNode> &deps, C  /*composition*/)
{
    ast::LiteralNode result = ast::LiteralNode(ast::LiteralType::NIL, false);
    std::for_each(deps.begin(), deps.end(),
        [&](ast::LiteralNode &in) { result = C(in); });

    return result;
}

ast::LiteralVariant addInt(ast::LiteralVariant &sum, int term)
{
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

ast::LiteralVariant subInt(ast::LiteralVariant &diff, int term)
{
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

ast::LiteralVariant addDouble(ast::LiteralVariant &sum, double term)
{
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

ast::LiteralVariant subDouble(ast::LiteralVariant &diff, double term)
{
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

ast::LiteralVariant multiply(ast::LiteralVariant &prod, int term)
{
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

ast::LiteralVariant multiply(ast::LiteralVariant &prod, double term)
{
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

ast::LiteralVariant divide(ast::LiteralVariant &diff, int term)
{
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

ast::LiteralVariant divide(ast::LiteralVariant &div, double term)
{
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
    std::for_each(deps.begin(), deps.end(), [&](ast::LiteralNode &in) {
        std::visit(
            [&](auto &arg) {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, int>) {
                    empty = false;
                    sum = addInt(sum, arg);
                } else if constexpr (std::is_same_v<T, double>) {
                    empty = false;
                    sum = addDouble(sum, arg);
                } else if constexpr (std::is_same_v<T, std::string>) {
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
            },
            in.literal);
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
    std::for_each(deps.begin() + 1, deps.end(), [&](ast::LiteralNode &in) {
        std::visit(
            [&](auto &arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>) {
                    diff = subInt(diff, arg);
                } else if constexpr (std::is_same_v<T, double>) {
                    diff = subDouble(diff, arg);
                } else {
                    throw TypeMismatchException();
                }
            },
            in.literal);
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

    std::for_each(deps.begin(), deps.end(), [&](ast::LiteralNode &in) {
        std::visit(
            [&](auto &arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>) {
                    prod = multiply(prod, arg);
                } else if constexpr (std::is_same_v<T, double>) {
                    prod = multiply(prod, arg);
                } else {
                    throw TypeMismatchException();
                }
            },
            in.literal);
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
    std::for_each(deps.begin() + 1, deps.end(), [&](ast::LiteralNode &in) {
        std::visit(
            [&](auto &arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>) {
                    div = divide(div, arg);
                } else if constexpr (std::is_same_v<T, double>) {
                    div = divide(div, arg);
                } else {
                    throw TypeMismatchException();
                }
            },
            in.literal);
    });

    if (std::holds_alternative<int>(div)) {
        return ast::LiteralNode(ast::LiteralType::INTEGER, div);
    } else if (std::holds_alternative<double>(div)) {
        return ast::LiteralNode(ast::LiteralType::REAL, div);
    } else {
        throw TypeMismatchException();
    }
}

ast::LiteralNode proc_car(std::vector<ast::LiteralNode> &deps)
{
    if (deps.empty()) {
        throw ArgLengthException();
    } else if (deps.size() == 1
        && deps[0].token_type == ast::LiteralType::LIST) {
        auto list = std::get<std::vector<ast::LiteralNode>>(deps[0].literal);
        return list[0];
    }

    return deps[0];
}

ast::LiteralNode proc_cdr(std::vector<ast::LiteralNode> &deps)
{
    auto list = std::vector<ast::LiteralNode>();
    if (deps.empty())
        throw ArgLengthException();
    else if (deps.size() == 1) {
        auto contents = std::get<std::vector<ast::LiteralNode>>(
            deps[0].literal);
        for (const auto& node : offset(contents, 1))
            list.emplace_back(node);
    }

    return ast::LiteralNode(list);
}

ast::LiteralNode proc_eq(std::vector<ast::LiteralNode> &deps)
{
    auto first = *deps.begin();
    bool eq = std::all_of(deps.begin() + 1, deps.end(),
        [&](ast::LiteralNode &in) -> bool { return in == first; });
    return ast::LiteralNode(ast::LiteralType::BOOL, eq);
}

ast::LiteralNode proc_atom(std::vector<ast::LiteralNode> &deps)
{
    bool atom = true;
    if (!deps.empty())
        if (deps[0].token_type == ast::LiteralType::LIST
            || deps[0].token_type == ast::LiteralType::NIL)
            atom = false;

    return ast::LiteralNode(ast::LiteralType::BOOL, atom);
}

ast::LiteralNode proc_null(std::vector<ast::LiteralNode> &deps)
{
    bool null = deps.empty();
    return ast::LiteralNode(ast::LiteralType::BOOL, null);
}

ast::LiteralNode proc_quote(ast::ListNodePtr list)
{
    return ast::LiteralNode(std::move(list));
}

ast::LiteralNode proc_def(
    const std::shared_ptr<env::Environment>& env, std::vector<ast::LiteralNode> &deps)
{
    if (deps.size() == 2) {
        if (deps[0].token_type != ast::LiteralType::IDENT)
            throw TypeMismatchException();

        auto tok = std::get<std::string>(deps[0].literal);
        env->addSymbol(tok,
            std::make_shared<env::Symbol>(
                std::make_shared<ast::LiteralNode>(deps[1]),
                env::SymbolType::CONST));
    }
    return deps[1];
}
}; // namespace depp
