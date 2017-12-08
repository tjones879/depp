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

class Applicable : public Symbol {
    using funcType = std::function<ast::LiteralNode(std::vector<ast::LiteralNode> &)>;
    funcType method;
public:
    Applicable(funcType func) : Symbol(nullptr) { 
        method = func;
    }
    ast::LiteralNode apply(std::vector<ast::LiteralNode> &deps) const;
};

class Value {
    std::string name;
};

class Environment {
    Environment *parent;
    std::unordered_map<std::string, Symbol> symbols;
public:
    Environment() : parent(nullptr) {}
    Environment(Environment *outer) : parent(outer) {}
    void addSymbol(std::string key, Symbol symbol);
    const Environment *findSymbol(const std::string &key) const;
    const Symbol &getSymbol(const std::string &key) const;
    void print(std::ostream &out) const;
};

typedef std::shared_ptr<Environment> EnvironmentPtr;
typedef std::shared_ptr<const Environment> SafeEnv;

}

#endif
