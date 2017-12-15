#ifndef BUILTIN_H
#define BUILTIN_H

#include "inc/ast.h"
#include "inc/env.hpp"
#include <vector>

namespace depp {
class TypeMismatchException : public std::runtime_error {
public:
    TypeMismatchException()
        : runtime_error("There were different types.")
    {
    }
    TypeMismatchException(std::string msg)
        : runtime_error(msg.c_str())
    {
    }
};

class ArgLengthException : public std::runtime_error {
public:
    ArgLengthException()
        : runtime_error("There were different types.")
    {
    }
    ArgLengthException(std::string msg)
        : runtime_error(msg.c_str())
    {
    }
};

ast::LiteralNode proc_add(std::vector<ast::LiteralNode> &deps);
ast::LiteralNode proc_sub(std::vector<ast::LiteralNode> &deps);
ast::LiteralNode proc_mult(std::vector<ast::LiteralNode> &deps);
ast::LiteralNode proc_div(std::vector<ast::LiteralNode> &deps);
ast::LiteralNode proc_car(std::vector<ast::LiteralNode> &deps);

ast::LiteralNode proc_cdr(std::vector<ast::LiteralNode> &deps);
ast::LiteralNode proc_eq(std::vector<ast::LiteralNode> &deps);
ast::LiteralNode proc_atom(std::vector<ast::LiteralNode> &deps);

ast::LiteralNode proc_null(std::vector<ast::LiteralNode> &deps);

ast::LiteralNode proc_quote(ast::ListNodePtr list);
ast::LiteralNode proc_def(
    const std::shared_ptr<env::Environment>& env, std::vector<ast::LiteralNode> &deps);
}

#endif
