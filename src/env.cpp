#include "inc/env.hpp"

namespace env {

ast::LiteralNode Applicable::apply(std::vector<ast::LiteralNode> &deps) const {
    return method(deps);
}

void Environment::addSymbol(std::string key, Symbol symbol) {
    if (symbols.find(key) == symbols.end())
        symbols[key] = symbol;
}

const Environment *Environment::findSymbol(const std::string &key) const {
    const Environment *ret = nullptr, *env = this;
    do {
        if (env->symbols.find(key) != symbols.end())
            ret = env;

        env = env->parent;
    } while (!ret && env);
    return ret;
}

const Symbol &Environment::getSymbol(const std::string &key) const {
    const Environment *env = findSymbol(key);
    return env->symbols.at(key);
}

void Environment::print(std::ostream &out) const {
    for (auto symbol : symbols)
        std::get<1>(symbol).print(out);
}


}
