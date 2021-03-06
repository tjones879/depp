#include <utility>

#include "inc/env.hpp"

namespace env {

ast::LiteralNode Applicable::apply(std::vector<ast::LiteralNode> &deps) const
{
    return method(deps);
}

void Environment::addSymbol(const std::string& key, SymbolPtr symbol)
{
    if (symbols.find(key) == symbols.end())
        symbols[key] = std::move(symbol);
}

const Environment *Environment::findSymbol(const std::string &key) const
{
    const Environment *ret = nullptr, *env = this;
    do {
        if (env->symbols.find(key) != symbols.end())
            ret = env;

        env = env->parent;
    } while ((ret == nullptr) && (env != nullptr));
    return ret;
}

std::shared_ptr<Symbol> Environment::getSymbol(const std::string &key) const
{
    const Environment *env = findSymbol(key);
    return env->symbols.at(key);
}

void Environment::print(std::ostream &out) const
{
    for (auto symbol : symbols) {
        out << std::get<0>(symbol) << " ";
        std::get<1>(symbol)->print(out);
    }
}

Applicable buildDef(std::shared_ptr<Environment> env,
    std::function<ast::LiteralNode(
        std::shared_ptr<Environment>, std::vector<ast::LiteralNode> &)>
        def)
{
    using namespace std::placeholders;
    auto f1 = std::bind(def, env, _1);
    return Applicable(f1);
}

Applicable buildFunc(std::shared_ptr<Environment> env, ast::NodePtr root,
    std::function<ast::LiteralNode(
        std::shared_ptr<Environment>, ast::LiteralNode, ast::NodePtr)>
        func)
{
    using namespace std::placeholders;
    auto f1 = std::bind(func, env, _1, root);
    return Applicable(f1);
}
} // namespace env
