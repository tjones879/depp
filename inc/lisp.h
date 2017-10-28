#ifndef LISP_H
#define LISP_H

#include "ast.h"

#define YYSTYPE ast::NodePtr

extern YYSTYPE yylval;

#endif /* LISP_H */
