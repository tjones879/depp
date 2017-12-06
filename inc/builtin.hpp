#ifndef BUILTIN_H
#define BUILTIN_H

#include "inc/ast.h"
#include <vector>

namespace depp {

ast::LiteralNode proc_add(std::vector<ast::LiteralNode> &deps);

}

#endif
