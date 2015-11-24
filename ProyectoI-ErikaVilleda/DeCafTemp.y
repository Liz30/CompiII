%{
#include <cstdio>
#include <string>
#include "ast.h"

using namespace std;

int yylex();
extern int line;

enum type_t { INT, BOOL};

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
    Statement *statement_t;
    Expr *expr_t;
    ExprList *exprL_t;
    IdExpr *idlist_t;
}

%token<num_t> NUM_INT
%token<id_t> ID STRING_LITERAL
%token KW_BOOL KW_BREAK KW_PRINT KW_READ KW_CONTINUE KW_CLASS
%token KW_ELSE KW_EXTENDS KW_FALSE KW_FOR KW_IF KW_INT KW_NEW
%token KW_NULL KW_RETURN KW_ROT KW_TRUE KW_VOID KW_WHILE KW_MAIN
%token TK_EOF TK_IGUAL TK_SUMA TK_RESTA TK_MULT TK_DIV TK_MOD TK_ASIG
%token TK_NEG TK_BIZQ TK_BDER OP_AND OP_OR OP_LT OP_LTE OP_NE OP_GT OP_GTE
%type<expr_t> expra expr term factor argument idL
%type<statement_t> list_st st opt_else block assign declaration
%type<statement_t> method_dec header declarationList methodList method_void
%type<exprL_t> arg_list
%type<num_t> array


%%

input: KW_CLASS ID '{' header '}' { input = $4; }
;

header: declarationList method_void methodList { $$ = new ProgramStatement($1, $2, $3); }
;

declarationList: declarationList declaration { $$ = $1;}
              | declaration { $$ = $1; }
              |             { $$ = 0; }
;

declaration: KW_INT idL ';' { $$ = new DeclarationStatement(0, $2); }
          |  KW_BOOL idL ';' { $$ = new DeclarationStatement(1, $2); }
;

idL: idL ',' ID array { $$ = $1;}
    | ID array {
                  int t = $2;
                  string id=$1;
                  free($1);
                  if (t == 0)
                  {
                    $$ = new IdExpr(id);
                  }
                  else {
                    $$ = new ArrayExpr(id, t);
                  }
                }
;

array: '[' NUM_INT ']' { $$ = $2; }
    |                  { $$ = 0;}
;

method_void: KW_VOID KW_MAIN '('')' block { $$ = new MethodStatement(9, "main", 0 , $5); }

methodList: methodList method_dec { $$ = $1; }
          | method_dec { $$ = $1; }
          |            {$$ = 0; }

method_dec: KW_INT ID '(' declaration ')' block {
                                                  string id=$2;
                                                  free($2);
                                                  $$ = new MethodStatement(0, id, $4, $6);
                                                }
        |   KW_BOOL ID '(' declaration ')' block {
                                                    string id=$2;
                                                    free($2);
                                                    $$ = new MethodStatement(1, id, $4, $6);
                                                 }
;

block: '{' list_st '}'  { $$ = $2; }
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

st: | KW_PRINT arg_list ';'      { $$ = new PrintStatement($2); }
    | assign { $$ = $1; }
    | KW_IF '(' expr ')' block opt_else    { $$ = new IfStatement($3, $5, $6); }
    | KW_WHILE '(' expr ')' block { $$ = new WhileStatement($3, $5); }
    | KW_FOR '(' ID TK_ASIG expr ';' expr ';' ID TK_ASIG expr ')' block {
                                                          string id_i = $3;
                                                          free($3);

                                                          string id_c = $9;
                                                          free($9);

                                                          $$ = new ForStatement (new AssignStatement(id_i, $5), $7, new AssignStatement(id_c, $11), $13);
                                                      }
;

assign: ID TK_ASIG expr ';' {
                              string id = $1;
                              free($1);
                              $$ = new AssignStatement(id, $3);
                            }

;

arg_list: arg_list ',' argument {
                            	    $$ = $1;
                            	    $$->push_back($3);
                            	  }
	      | argument {
                	    $$ = new ExprList;
                	    $$->push_back($1);
                	}
;

argument: expr { $$ = $1; }
	     |  STRING_LITERAL {
	                         string str = $1;
	                         free($1);
                           $$ = new StringExpr(str);
	                      }
;

opt_else: KW_ELSE block    { $$ = $2; }
        |               { $$ = 0; }
;

expr: expr OP_LT expra  { $$ = new LTExpr($1, $3); }
    | expr OP_GT expra  { $$ = new GTExpr($1, $3); }
    | expr OP_LTE expra { $$ = new LTEExpr($1, $3); }
    | expr OP_GTE expra { $$ = new GTEExpr($1, $3); }
    | expr OP_NE expra  { $$ = new NEExpr($1, $3); }
    | expr TK_ASIG expra    { $$ = new EQExpr($1, $3);; }
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
        | NUM_INT           { $$ = new NumExpr($1); }
        | ID            {
                            string id = $1;
                            free($1);
                            $$ = new IdExpr(id);
                        }
        | KW_TRUE     { $$ = new BoolExpr("true"); }
        | KW_FALSE    { $$ = new BoolExpr("false"); }
;

%%
