%option noyywrap c++

%{
#include <iostream>
#include <stdio.h>
#include <math.h>

#include "inc/lisp.h"
#include "parser.hpp"

#define DEBUG_SCANNER 0
%}

SIGN "-"|"+"
ALPHA [a-zA-Z]
DIGIT [0-9]
SYMBOL "-"|"+"|"/"|"*"|"<"|">"|"="|"!"|"@"|"#"|"$"|"%"|"^"|"&"

%%
(;.*) {
        #if DEBUG_SCANNER
        printf("A comment: %s\n", yytext);
        #endif
    }

":" {
        #if DEBUG_SCANNER
        printf("COLON: %s\n", yytext);
        #endif
        return COLON;
    }

"("|")"|"{"|"}"|"["|"]"|"\\" {
        #if DEBUG_SCANNER
        printf("%s\n", yytext);
        #endif
        return *yytext;
    }

{SIGN}?{DIGIT}+ {
        yylval = ast::node(ast::LiteralType::INTEGER, atoi(yytext));
        #if DEBUG_SCANNER
        std::cout << "lexed " << yytext << " as INT: " << yylval;
        #endif
        return INTEGER;
    }

{SIGN}?{DIGIT}+"."{DIGIT}*((e|E){SIGN}?{DIGIT}+)? {
        yylval = ast::node(ast::LiteralType::REAL, atof(yytext));
        #if DEBUG_SCANNER
        std::cout << "lexed " << yytext << " as FLOAT: " << yylval;
        #endif
        return FLOAT;
    }

(nil) {
        yylval = ast::node(ast::LiteralType::NIL, 0);
        #if DEBUG_SCANNER
        std::cout << "lexed " << yytext << " as NIL";
        #endif
        return NIL;
    }

(false|true) {
        // TODO Convert string to bool
        yylval = ast::node(ast::LiteralType::BOOL, yytext);
        #if DEBUG_SCANNER
        std::cout << "lexed " << yytext << " as BOOL";
        #endif
        return BOOL;
    }

(def|fn|defn|if|while|when|do|lambda|quote|"'"|{SYMBOL}) {
        yylval = ast::node(ast::LiteralType::RESERVED, std::string(yytext));
        #if DEBUG_SCANNER
        std::cout << "lexed " << yytext << " as RESERVED: " << yylval;
        #endif
        return RESERVED;
    }

({ALPHA}|"_")({SIGN}|{ALPHA}|{DIGIT}|{SYMBOL}|"_")* {
        yylval = ast::node(ast::LiteralType::IDENT, std::string(yytext));
        #if DEBUG_SCANNER
        std::cout << "lexed " << yytext << " as IDENT: " << yylval;
        #endif
        return IDENT;
    }

\"(\\.|[^"\\])*\" {
        yylval = ast::node(ast::LiteralType::STRING, std::string(yytext));
        #if DEBUG_SCANNER
        std::cout << "lexed " << yytext << " as STRING: " << yylval;
        #endif
        return STRING;
    }

[ \t\r\n] ;

. printf("UNRECOGNIZED: %s\n", yytext);
%%
