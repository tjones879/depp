#ifndef ENV_H
#define ENV_H

#include "inc/ast.h"
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

template <class F>
class Applicable : public Symbol {
    F method;
public:
    Applicable(ast::NodePtr node, F func) : Symbol(node), method(func) { }
    ast::LiteralNode apply(std::vector<ast::LiteralNode> &deps);
};

class Value {
    std::string name;
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
typedef std::shared_ptr<const Environment> SafeEnv;

}

#endif
