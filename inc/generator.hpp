#ifndef GENERATOR_H
#define GENERATOR_H

#include "inc/ast.h"
#include "inc/env.hpp"

namespace gen {

struct Generator {
    env::EnvironmentPtr env;
    ast::NodePtr ast;
    Generator(const ast::NodePtr ptr);
    Generator(env::EnvironmentPtr en, const ast::NodePtr &ptr);
    void dumpEnv(std::ostream &out);
    ast::LiteralNodePtr walkTree(const ast::NodePtr ptr);
    std::shared_ptr<env::Symbol> handleRunner(
        ast::NodePtr ptr, ast::NodePtr parent);
};
}

#endif
