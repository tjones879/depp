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
    virtual void generate()=0;
};

struct ReservedHandler : public Generator {
    ReservedHandler(const ast::NodePtr &ptr) : Generator(ptr) {}
    virtual void generate();
    void checkSymbols(ast::NodePtr parent, ast::NodePtr node, int position);
};
}

#endif
