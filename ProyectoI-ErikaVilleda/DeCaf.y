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
%type<expr_t> expra expr term factor argument arrayE exprOPT functionCall
%type<statement_t>  assignmentH declaration header method  st block elseOPT
%type<statement_t> assignmentE method_call
%type<sList_t> headerList methodsList declarationList stList assignEList
%type<eList_t> elist argumentList paramList idL
%type<num_t> array
%type<id_t> idVOID

%%

input: KW_CLASS ID '{' headerList methodsList '}' {
                                              string id=$2;
                                              free($2);
                                              input = new ProgramStatement (id, $4, $5); }
;

headerList: headerList header {
                                $$ = $1;
                                $$->push_back($2);
                              }
          | header {
                      $$ = new StatementList;
                      $$->push_back($1);
                   }
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
;

declarationList: declarationList declaration {
                                                $$ = $1;
                                                $$->push_back($2);
                                              }
              | declaration {
                              $$ = new StatementList;
                              $$->push_back($1);
                            }
              |             { $$ = 0; }
;

declaration: t_var elist ';' {
                                  $$ = new DeclarationStatement($1, $2);
                              }
;

elist: elist ',' ID array {
                        string id = $3;
                        free($3);
                        $$ = $1;
                        if ( $4 == 0 )
                          $$->push_back(new IdExpr(_NULL, id));
                        else
                          $$->push_back(new ArrayExpr(id, $4))
                      }
      | ID array {
              string id = $1;
              free($1);
              $$ = new ExprList;
              if ( $2 == 0 )
                $$->push_back(new IdExpr(_NULL, id));
              else
                $$->push_back(new ArrayExpr(id, $2))
            }
      |     { $$ = 0; }
;

array: '[' NUM_INT ']'  { $$ = $2; }
      |                 { $$ = 0; }
;

methodsList: methodsList method {
                                  $$ = $1;
                                  $$->push_back($2);
                                }
          | method {
                      $$ = new StatementList;
                      $$->push_back($1);
                   }
;

method: t_var ID '(' argumentList ')' block {
                          string id = $2;
                          free($2);
                          $$ = new MethodStatement($1, id, $4, $6);
                        }
      | KW_VOID idVOID '(' argumentList ')' block {
                                                     string id = $2;
                                                     free($2);
                                                     $$ = new MethodStatement(VOID, id, $4, $6);
                                                  }
;

idVOID: ID {
              $$=$1;
            }
      | KW_MAIN { $$ = (char*)"MAIN";}
;

argumentList: argumentList ',' argument {
                                  $$ = $1;
                                  $$->push_back($3);
                                }
          | argument {
                      $$ = new ExprList;
                      $$->push_back($1);
                   }
          |        { $$ = 0;}
;

argument: t_var ID {
                      string id = $2;
                      free($2);
                      $$ = new IdExpr($1, id);
                    }
;

block:  '{' declarationList stList '}' {
                                          $$ = new BlockStatement($2, $3);
                                       }
;

stList: stList st {
                    $$ = $1;
                    $$->push_back($2);
                  }

        | st        {
                      $$ = new StatementList;
                      $$->push_back($1);
                    }
        |           { $$ = 0; }
;

st: assignmentE ';' { $$ = $1; }
  | method_call ';' { $$ = $1; }
  | KW_IF '(' expr ')' block elseOPT { $$ = new IfStatement($3, $5, $6); }
  | KW_WHILE '(' expr ')' block { $$ = new WhileStatement($3, $5); }
  | KW_FOR '(' paramList ';' expr ';' assignEList  ')' block { $$ = new ForStatement($3, $5, $7, $9); }
  | KW_RETURN exprOPT ';' { $$ = new ReturnStatement($2); }
  | KW_BREAK ';' { $$ = new BreakStatement(); }
  | KW_CONTINUE ';' { $$ = new ContinueStatement(); }
  | block { $$ = $1; }
;

exprOPT: expr { $$ = $1; }
      |       { $$ = 0; }

assignEList: assignEList ',' assignmentE {
                                            $$ = $1;
                                            $$->push_back($3);
                                          }
          | assignmentE {
                          $$ = new StatementList;
                          $$->push_back($1);
                        }
;

elseOPT: KW_ELSE block { $$ = $2; }
      |                { $$ = 0; }
;

method_call: ID '(' paramList ')' {
                                    string id = $1;
                                    free($1);
                                    $$ = new MethodCallStatement(id, $3);
                                  }
          | KW_PRINT  paramList { $$ = new PrintStatement ($2); }
          | KW_READ idL         { $$ = new ReadStatement($2); }
;

idL: idL ',' ID arrayE {
                          string id = $3;
                          free($3);
                          $$ = $1;
                          if ($4 == 0)
                            $$->push_back(new IdExpr(_NULL, id));
                          else
                            $$->push_back(new ArrayExpr(id,(int)$4));
                        }
      | ID arrayE {
                    $$ = new ExprList;
                    string id = $1;
                    free($1);
                    if ($2 == 0)
                      $$->push_back(new IdExpr(_NULL, id));
                    else
                      $$->push_back(new ArrayExpr(id,(int)$2));
                   }
      |            { $$ = 0; }
;

paramList: paramList ',' expr {
                                  $$ = $1;
                                  $$->push_back($3);
                                }
        | expr                 {
                                  $$ = new ExprList;
                                  $$->push_back($1);
                                }
        |                       { $$ = 0; }
;

assignmentE: ID arrayE TK_ASIG expr {
                                      string id = $1;
                                      free($1);
                                      if ($2==0)
                                        $$ = new AssignStatement(_NULL, id, $4);
                                      else
                                      {
                                        $$ = new AssignStatementArray(_NULL, id, $2, $4);
                                      }
                                    }
;

arrayE: '[' expr ']' { $$ = $2; }
      |     { $$ = 0; }
;

functionCall: ID '(' paramList ')' {
                                      string id = $1;
                                      free($1);
                                      $$ = new FunctionCallExpr(id, $3);
                                    }
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

expra: expra TK_SUMA term   { $$ = new AddExpr($1, $3); }
    | expra TK_RESTA term    { $$ = new SubExpr($1, $3); }
    | term              { $$ = $1; }
;

term: term TK_MULT factor   { $$ = new MultExpr($1, $3); }
    | term TK_DIV factor   { $$ = new DivExpr($1, $3); }
    | term TK_MOD factor   { $$ = new ModExpr ($1, $3); }
    | factor            { $$ = $1; }
;

factor: '(' expr ')'    { $$ = $2; }
        | NUM_INT       { $$ = new NumExpr($1); }
        | ID arrayE     {
                            string id = $1;
                            free($1);
                            if ($2 == 0)
                                $$ = new IdExpr(_NULL, id);
                            else
                                $$ = new ArrayExpr(id, (int)$2);
                        }
        | STRING_LITERAL {
                            string strl = $1;
                            free($1);
                            $$ = new StringExpr(strl);
                          }
        | KW_TRUE     { $$ = new BoolExpr("true"); }
        | KW_FALSE    { $$ = new BoolExpr("false"); }
        | functionCall { $$ = $1; }
;
