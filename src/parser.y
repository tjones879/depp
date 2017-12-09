%{
#include <stdio.h>
#include <string.h>
#include <FlexLexer.h>
#include "inc/lisp.h"
#include "inc/env.hpp"
#include "inc/builtin.hpp"
#include "inc/generator.hpp"

#define DEBUG_PARSER 0

yyFlexLexer lex;
void yyerror(const char *str);
int yywrap();
int yylex() { return lex.yylex(); }

ast::NodePtr program(0);
%}

%token INTEGER FLOAT NIL BOOL RESERVED IDENT STRING L_PAREN R_PAREN L_BRACE R_BRACE L_CURLY R_CURLY COLON

%%

program:
    atoms
    {
        #if DEBUG_PARSER
        std::cout << std::endl << "program <- atoms " << std::endl;
        #endif
        program = ast::NodePtr(new ast::ProgramNode());
        for (auto &child : $1->children)
            program->children.push_back(child);
    };

atoms:
    {
        #if DEBUG_PARSER
        std::cout << "atoms <- EMPTY " << std::endl;
        #endif
        $$ = ast::NodePtr(new ast::AtomsNode());
    }
    |
    atoms atom
    {
        #if DEBUG_PARSER
        std::cout << "atoms <- atoms :" << $1 << " atom :" << $2 << std::endl;
        #endif
        $$ = $1;
        $$->children.push_back($2);
    };


atom:
    literal
    {
        #if DEBUG_PARSER
        std::cout << "atom <- literal " << $1 << std::endl;
        #endif
    }
    | list
    {
        #if DEBUG_PARSER
        std::cout << "atom <- list " << $1 << std::endl;
        #endif
    }
    | vector
    {
        #if DEBUG_PARSER
        std::cout << "atom <- vector " << $1 << std::endl;
        #endif
    }
    | map
    {
        #if DEBUG_PARSER
        std::cout << "atom <- map " << $1 << std::endl;
        #endif
    };

literal:
    INTEGER
    {
        #if DEBUG_PARSER
        std::cout << "literal <- INTEGER " << $1 << std::endl;
        #endif
        $$=$1;
    }
    | FLOAT
    {
        #if DEBUG_PARSER
        std::cout << "literal <- FLOAT " << $1 << std::endl;
        #endif
        $$=$1;
    }
    | BOOL
    {
        #if DEBUG_PARSER
        std::cout << "literal <- BOOL " << $1 << std::endl;
        #endif
        $$=$1;
    }
    | NIL
    {
        #if DEBUG_PARSER
        std::cout << "literal <- NIL " << $1 << std::endl;
        #endif
        $$=$1;
    }
    | STRING
    {
        #if DEBUG_PARSER
        std::cout << "literal <- STRING " << $1 << std::endl;
        #endif
        $$=$1;
    }
    | RESERVED
    {
        #if DEBUG_PARSER
        std::cout << "literal <- RESERVED " << $1 << std::endl;
        #endif
        $$=$1;
    }
    | COLON IDENT
    {
        #if DEBUG_PARSER
        std::cout << "literal <- keyword " << $2 << std::endl;
        #endif
        std::dynamic_pointer_cast<ast::LiteralNode>($2)->token_type = ast::LiteralType::KEYWORD;
        $$ = $2;
    }
    | IDENT
    {
        #if DEBUG_PARSER
        std::cout << "literal <- IDENT " << $1 << std::endl;
        #endif
        $$=$1;
    };

list:
    '(' atoms ')'
    {
        #if DEBUG_PARSER
        std::cout << "list <- atoms " << $2 << std::endl;
        #endif
        $$ = ast::NodePtr(ast::node(ast::NodeType::LIST));
        for (auto &child : $2->children)
            $$->children.push_back(child);
    };

vector:
    '[' atoms ']'
    {
        #if DEBUG_PARSER
        std::cout << "vector <- atoms " << $2 << std::endl;
        #endif
        $$ = ast::NodePtr(ast::node(ast::NodeType::VECTOR));
        for (auto &child : $2->children)
            $$->children.push_back(child);
    };

map:
    '{' atoms '}'
    {
        #if DEBUG_PARSER
        std::cout << "map <- atoms " << $2 << std::endl;
        #endif
        $$ = ast::NodePtr(ast::node(ast::NodeType::MAP));
        for (auto &child : $2->children)
            $$->children.push_back(child);
    };
%%

int yywrap()
{
    return 1;
}

void yyerror(const char *str)
{
    fprintf(stderr, "error %s\n", str);
}

env::EnvironmentPtr buildGlobalEnv()
{
    auto global = std::make_shared<env::Environment>();
    global->addSymbol("+", std::make_shared<env::Applicable>(depp::proc_add));
    global->addSymbol("-", std::make_shared<env::Applicable>(depp::proc_sub));
    return global;
}

int main()
{
    yyparse();
    if (program) {
        std::cout << program << std::endl;
        auto env = buildGlobalEnv();
        gen::Generator res(env, program);
        res.walkTree(program);
    } else {
        std::cout << "FAILURE: syntax error." << std::endl;
    }
    return 0;
}
