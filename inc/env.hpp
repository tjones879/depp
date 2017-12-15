#ifndef ENV_H
#define ENV_H

#include "inc/ast.h"
#include <string>
#include <unordered_map>

namespace env {

enum class SymbolType : uint8_t {
    CONST = 0,
    FUNC = 1,
    NIL = 2,
};

class Symbol {
    ast::LiteralNodePtr val;

public:
    SymbolType type;
    Symbol()
        : val(nullptr)
        , type(SymbolType::NIL)
    {
    }
    Symbol(ast::LiteralNodePtr node, SymbolType stype)
        : val(node)
        , type(stype)
    {
    }
    ast::LiteralNodePtr getVal()
    {
        return val;
    }
    virtual void print(std::ostream &out)
    {
        val.get()->print(out);
    }
};

typedef std::shared_ptr<Symbol> SymbolPtr;

class Applicable : public Symbol {
    using funcType = std::function<ast::LiteralNode(
        std::vector<ast::LiteralNode> &)>;
    funcType method;

public:
    Applicable(funcType func)
        : Symbol(nullptr, SymbolType::FUNC)
    {
        method = func;
    }
    ast::LiteralNode apply(std::vector<ast::LiteralNode> &deps) const;
    void print(std::ostream &out)
    {
        out << "func" << std::endl;
    }
};

class Value {
    std::string name;
};

class Environment {
    Environment *parent;
    std::unordered_map<std::string, SymbolPtr> symbols;

public:
    Environment()
        : parent(nullptr)
    {
    }
    Environment(Environment *outer)
        : parent(outer)
    {
    }
    void addSymbol(std::string key, SymbolPtr symbol);
    const Environment *findSymbol(const std::string &key) const;
    SymbolPtr getSymbol(const std::string &key) const;
    void print(std::ostream &out) const;
};

typedef std::shared_ptr<Environment> EnvironmentPtr;
typedef std::shared_ptr<const Environment> SafeEnv;

Applicable buildDef(std::shared_ptr<Environment> env,
    std::function<ast::LiteralNode(
        std::shared_ptr<Environment>, std::vector<ast::LiteralNode> &)>
        def);

using defun = std::function<ast::LiteralNode(
    std::shared_ptr<Environment>, ast::LiteralNode args, ast::NodePtr subtree)>;

Applicable buildFunc(
    std::shared_ptr<Environment> env, ast::NodePtr root, defun func);
}

#endif
