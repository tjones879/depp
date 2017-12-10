#ifndef BUILTIN_H
#define BUILTIN_H

#include "inc/ast.h"
#include "inc/env.hpp"
#include <vector>

namespace depp {

ast::LiteralNode proc_add(std::vector<ast::LiteralNode> &deps); // WORKING
ast::LiteralNode proc_sub(std::vector<ast::LiteralNode> &deps); // WORKING
ast::LiteralNode proc_mult(std::vector<ast::LiteralNode> &deps); // WORKING
ast::LiteralNode proc_div(std::vector<ast::LiteralNode> &deps); // WORKING
ast::LiteralNode proc_car(std::vector<ast::LiteralNode> &deps); // WORKING

ast::LiteralNode proc_cdr(std::vector<ast::LiteralNode> &deps); // WORKING
ast::LiteralNode proc_eq(std::vector<ast::LiteralNode> &deps); // NOT WORKING

ast::LiteralNode proc_null(std::vector<ast::LiteralNode> &deps); // WORKING

ast::LiteralNode proc_quote(ast::ListNodePtr list); // WORKING (NOT WHEN FIRST ATOM)
ast::LiteralNode proc_def(std::shared_ptr<env::Environment> env, std::vector<ast::LiteralNode> &deps);

}

#endif
