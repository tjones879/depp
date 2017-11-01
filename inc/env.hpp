#ifndef ENV_H
#define ENV_H

#include "ast.h"
#include <unordered_map>
#include <string>

namespace env {

class Symbol {
    ast::NodePtr val;
public:
    Symbol() : val(nullptr) { }
    Symbol(ast::NodePtr node) : val(node) { }
    void print(std::ostream &out) {
        val.get()->print(out);
    }
};

class Environment {
    Environment *parent;
    std::unordered_map<std::string, Symbol> symbols;
public:
    Environment();
    Environment(Environment *outer) : parent(outer) {}
    void addSymbol(std::string key, Symbol symbol);
    const Environment *findSymbol(const std::string &key);
    const Symbol &getSymbol(const std::string &key);
    void print(std::ostream &out);
};

typedef std::shared_ptr<Environment> EnvironmentPtr;

}

#endif
