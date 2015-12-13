%{
#include <cstdio>
#include <string>
#include "ast.h"

using namespace std;

int yylex();
extern int line;

void yyerror(const char *str)
{
    printf("Error: %s\nLinea: %d\n", str, line+1);
}

#define YYERROR_VERBOSE 1

Statement *input;

%}

%union {
    char *id_t;
    int  num_t;
    Statement *statement_t;
    Expr *expr_t;
    Type_v t;
    StatementList *sList_t;
    ExprList *eList_t;
}


%token<num_t> NUM_INT
%token<id_t> ID STRING_LITERAL
%token KW_BOOL KW_BREAK KW_PRINT KW_READ KW_CONTINUE KW_CLASS
%token KW_ELSE KW_EXTENDS KW_FALSE KW_FOR KW_IF KW_INT KW_NEW
%token KW_NULL KW_RETURN KW_ROT KW_TRUE KW_VOID KW_WHILE KW_MAIN
%token TK_EOF TK_IGUAL TK_SUMA TK_RESTA TK_MULT TK_DIV TK_MOD TK_ASIG
%token TK_NEG TK_BIZQ TK_BDER OP_AND OP_OR OP_LT OP_LTE OP_NE OP_GT OP_GTE
%type<t> t_var
%type<expr_t> expra expr term factor
%type<statement_t>  assignmentH declaration header
%type<sList_t> headerList
%type<eList_t> elist
%type<num_t> array

%%

input: KW_CLASS ID '{' headerList '}' {
                                              string id=$2;
                                              free($2);
                                              input = new ProgramStatement (id, $4, 0); }
;

headerList: headerList header {
                                $$ = $1;
                                $$->push_back($2);
                              }
          | header {
                      $$ = new StatementList;
                      $$->push_back($1);
                   }
         |         { $$ = 0; }
;

header: assignmentH  { $$ = $1; }
      | declaration  { $$ = $1; }
;

assignmentH: t_var ID TK_ASIG expr ';' {
                                          string id = $2;
                                          free($2);
                                          $$ = new AssignStatement($1, id, $4);
                                      }
;

t_var: KW_INT { $$ = INT; }
    | KW_BOOL { $$ = BOOL; }
    | KW_VOID { $$ = VOID; }
;

declaration: t_var elist ';' {
                                  $$ = new DeclarationStatement($1, $2);
                              }
;

elist: elist ',' ID array {
                        string id = $3;
                        free($3);
                        $$ = $1;
                        $$->push_back(new IdExpr(id));
                      }
      | ID array {
              string id = $1;
              free($1);
              $$ = new ExprList;
              if ( $2 == 0 )
                $$->push_back(new IdExpr(id));
              else
                $$->push_back(new ArrayExpr(id, $2))
            }
      |     { $$ = 0; }
;

array: '[' NUM_INT ']'  { $$ = $2; }
      |                 { $$ = 0; }
;

expr: expr OP_LT expra  { $$ = new LTExpr($1, $3); }
    | expr OP_GT expra  { $$ = new GTExpr($1, $3); }
    | expr OP_LTE expra { $$ = new LTEExpr($1, $3); }
    | expr OP_GTE expra { $$ = new GTEExpr($1, $3); }
    | expr OP_NE expra  { $$ = new NEExpr($1, $3); }
    | expr TK_IGUAL expra    { $$ = new EQExpr($1, $3); }
    | expr OP_OR expra  { $$ = 0; }
    | expr OP_AND expra { $$ = 0; }
    | TK_RESTA expr     { $$ =0 ;}
    | TK_NEG expr       { $$ = 0; }
    | expr KW_ROT expra { $$ = 0; }
    | expr TK_BIZQ expra { $$ = 0; }
    | expr TK_BDER expra  { $$ = 0; }
    | expra             { $$ = $1; }
;

expra: expra '+' term   { $$ = new AddExpr($1, $3); }
    | expra '-' term    { $$ = new SubExpr($1, $3); }
    | term              { $$ = $1; }
;

term: term '*' factor   { $$ = new MultExpr($1, $3); }
    | term '/' factor   { $$ = new DivExpr($1, $3); }
    | term '%' factor   { $$ = new ModExpr ($1, $3); }
    | factor            { $$ = $1; }
;

factor: '(' expr ')'    { $$ = $2; }
        | NUM_INT           { $$ = new NumExpr($1); }
        | ID            {
                            string id = $1;
                            free($1);
                            $$ = new IdExpr(id);
                        }
        | KW_TRUE     { $$ = new BoolExpr("true"); }
        | KW_FALSE    { $$ = new BoolExpr("false"); }
;
