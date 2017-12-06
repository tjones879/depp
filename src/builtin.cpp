#include "inc/builtin.hpp"
#include "inc/ast.h"
#include <vector>
#include <algorithm>
#include <type_traits>
#include <numeric>

namespace depp {

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

ast::LiteralNode proc_add(std::vector<ast::LiteralNode> &deps)
{
    ast::LiteralVariant sum(0);
    std::for_each(deps.begin(), deps.end(),
            [&] (ast::LiteralNode &in) {
                std::visit([&](auto &arg) {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, int>) {
                        auto rep = std::get<int>(sum);
                        sum = ast::LiteralVariant(rep + arg);
                    } else if constexpr(std::is_same_v<T, double>) {
                        auto rep = std::get<double>(sum);
                        sum = ast::LiteralVariant(rep + arg);
                    }
                }, in.literal);
            });
    return ast::LiteralNode(ast::LiteralType::REAL, sum);
}

ast::LiteralNode proc_sub(const std::vector<ast::NodePtr> &deps)
{
    auto first = *deps.begin();
}
}; // namespace depp
