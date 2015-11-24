%{
#include <cstdio>
#include <string>
#include "ast.h"

using namespace std;

int yylex();
extern int line;

void yyerror(const char *str)
{
    printf("Error: %s\nLinea: %d\n", str, line);
}

#define YYERROR_VERBOSE 1

Statement *input;

%}

%union {
    char *id_t;
    int  num_t;
    double num_d;
    Statement *statement_t;
    Expr *expr_t;
}

%token<num_t> NUM_INT
%token<id_t> ID STRING_LITERAL
%token<num_d> NUM_DOUBLE
%token TK_IGUAL TK_SUMA TK_RESTA TK_MULT TK_DIV TK_MOD TK_ASIG TK_NEG TK_BIZQ TK_BDER TK_EOF
%token KW_BOOL KW_BREAK KW_PRINT KW_READ KW_CONTINUE KW_CLASS KW_ELSE KW_EXTENDS KW_FALSE
%token KW_FOR KW_IF KW_INT KW_NEW NW_NULL KW_RETURN KW_ROT KW_TRUE KW_VOID KW_WHILE KW_MAIN
%token OP_LT OP_GT OP_LTE OP_GTE OP_NE OP_AND OP_OR
%type<expr_t> expra expr term factor
%type<statement_t> list_st st opt_else block

%%

input: list_st { input = $1; }
;

list_st: list_st st {
                        if ($1->getKind() == BLOCK_STATEMENT) {
                            BlockStatement *block = (BlockStatement*)$1;

                            block->stList.push_back($2);

                            $$ = block;
                        } else {
                            list<Statement *> l;

                            l.push_back($1);
                            l.push_back($2);

                            $$ = new BlockStatement(l);
                        }
                    }

        | st        { $$ = $1; }
;

st: KW_PRINT expr ';'      { $$ = new PrintStatement($2); }
    | ID '=' expr ';'   {
                          string id = $1;

                          free($1);

                          $$ = new AssignStatement(id, $3);
                        }
    | KW_IF '(' expr ')' block opt_else    { $$ = new IfStatement($3, $5, $6); }
    | KW_WHILE '(' expr ')' block { $$ = new WhileStatement($3, $5); }
    | KW_FOR '(' ID '=' expr ';' expr ';' ID '=' expr ')' block {
                                                          string id_i = $3;
                                                          free($3);

                                                          string id_c = $9;
                                                          free($9);

                                                          $$ = new ForStatement (new AssignStatement(id_i, $5), $7, new AssignStatement(id_c, $11), $13);
                                                      }
;

opt_else: KW_ELSE block    { $$ = $2; }
        |               { $$ = 0; }
;

block: '{' list_st '}'  { $$ = $2; }
;

expr: expr OP_LT expra  { $$ = new LTExpr($1, $3); }
    | expr OP_GT expra  { $$ = new GTExpr($1, $3); }
    | expr OP_LTE expra { $$ = new LTEExpr($1, $3); }
    | expr OP_GTE expra { $$ = new GTEExpr($1, $3); }
    | expr OP_NE expra  { $$ = new NEExpr($1, $3); }
    | expr '=' expra    { $$ = new EQExpr($1, $3);; }
    | expra             { $$ = $1; }
;

expra: expra '+' term   { $$ = new AddExpr($1, $3); }
    | expra '-' term    { $$ = new SubExpr($1, $3); }
    | term              { $$ = $1; }
;

term: term '*' factor   { $$ = new MultExpr($1, $3); }
    | term '/' factor   { $$ = new DivExpr($1, $3); }
    | factor            { $$ = $1; }
;

factor: '(' expr ')'    { $$ = $2; }
        | NUM_INT       { $$ = new NumExpr($1); }
        | ID            {
                            string id = $1;

                            free($1);
                            $$ = new IdExpr(id);
                        }
;

%%
