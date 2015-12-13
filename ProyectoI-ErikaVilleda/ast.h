#ifndef _AST_H_
#define _AST_H_

#include <string>
#include <list>
#include <map>

using namespace std;

extern map<string, int> vars_type;  // 0 Int; 1 Bool
extern map<string, int> vars;
extern map<string, int> nmethods;

enum Type_v { INT, BOOL, VOID, _NULL};

class Expr;
class Statement;
typedef list<Expr*> ExprList;
typedef list<Statement*> StatementList;


class Expr {
public:
    virtual int evaluate() = 0;
};

class BinaryExpr: public Expr {
public:
    BinaryExpr(Expr *expr1, Expr *expr2) {
        this->expr1 = expr1;
        this->expr2 = expr2;
    }

    Expr *expr1;
    Expr *expr2;
};

class LTExpr: public BinaryExpr {
public:
    LTExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() < expr2->evaluate(); }
};

class GTExpr: public BinaryExpr {
public:
    GTExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() > expr2->evaluate(); }
};

class LTEExpr: public BinaryExpr {
public:
    LTEExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() <= expr2->evaluate(); }
};

class GTEExpr: public BinaryExpr {
public:
    GTEExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() >= expr2->evaluate(); }
};

class NEExpr: public BinaryExpr {
public:
    NEExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() != expr2->evaluate(); }
};

class EQExpr: public BinaryExpr {
public:
    EQExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() == expr2->evaluate(); }
};

class ORExpr: public BinaryExpr{
public:
    ORExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() || expr2->evaluate(); }
};

class ANDExpr: public BinaryExpr{
public:
    ANDExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() && expr2->evaluate(); }
};

class AddExpr: public BinaryExpr {
public:
    AddExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() + expr2->evaluate(); }
};

class SubExpr: public BinaryExpr {
public:
    SubExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() - expr2->evaluate(); }
};

class MultExpr: public BinaryExpr {
public:
    MultExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() * expr2->evaluate(); }
};

class DivExpr: public BinaryExpr {
public:
    DivExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() / expr2->evaluate(); }
};

class ModExpr: public BinaryExpr{
public:
    ModExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() % expr2 -> evaluate(); }
};

class NumExpr: public Expr {
public:
    NumExpr(int value) { this->value = value; }
    int evaluate() { return value; }

    int value;
};

class IdExpr: public Expr {
public:
    IdExpr(string id) { this->id = id; }
    int evaluate() { return vars[id]; }

    string id;
};

class StringExpr: public Expr {
public:
    StringExpr(string &str) { this->str = str; }
    int evaluate() { return 0 ; }
    string str;
};

class BoolExpr: public Expr{
public:
    BoolExpr(string value) { this->value = value; }
    int evaluate() { if (value=="true") return 1; return 0;}
    string value;
};

class ArrayExpr: public Expr{
public:
    ArrayExpr(string id, int size){
                                    this->id=id;
                                    this->size = size;
                                  }
    int evaluate(){ return 0; } //Validar que size sea > 0
    string id;
    int size;
    //int type; // Todos los arreglos los manejo tipo enteros
};

class FunctionCallExpr: public Expr{
public:
    FunctionCallExpr(string id, ExprList *param){
      this->id=id;
      this->param;
  }

  int evaluate() {//recorrer param y evaluar//
                  return 0; }

  string id;
  ExprList *param;
};


enum StatementKind {
    BLOCK_STATEMENT,
    PRINT_STATEMENT,
    ASSIGN_STATEMENT,
    IF_STATEMENT,
    WHILE_STATEMENT,
    FOR_STATEMENT,
    DECLARATION_STATEMENT,
    PROGRAM_STATEMENT,
    METHOD_STATEMENT,
    METHODCALL_STATEMENT,
    READ_STATEMENT,
    RETURN_STATEMENT
};

class Statement {
public:
    virtual void execute() = 0;
    virtual StatementKind getKind() = 0;
};

class BlockStatement: public Statement {
public:
    BlockStatement(list<Statement *> stList) { this->stList = stList; }
    void execute();
    StatementKind getKind() { return BLOCK_STATEMENT; }

    list<Statement *> stList;
};

class PrintStatement: public Statement {
public:
    PrintStatement(ExprList *expr) { this->expr = expr; }
    void execute();
    StatementKind getKind() { return PRINT_STATEMENT; }

    ExprList *expr;
};

class AssignStatement: public Statement {
public:
    AssignStatement(Type_v type_v, string id, Expr *expr) {
        this->type_v = type_v;
        this->id = id;
        this->expr = expr;
    }

    void execute();
    StatementKind getKind() { return ASSIGN_STATEMENT; }

    Type_v type_v;
    string id;
    Expr *expr;
};

class AssignStatementArray: public Statement {
public:
    AssignStatementArray(string id, int pos, Expr *expr){
        this->id = id;
        this->pos = pos;
        this->expr = expr;
    }
    void execute();
    StatementKind getKind() { return ASSIGN_STATEMENT; }

    string id;
    Expr *expr;
    int pos;
};

class IfStatement: public Statement {
public:
    IfStatement(Expr *cond, Statement *trueBlock, Statement *falseBlock) {
        this->cond = cond;
        this->trueBlock = trueBlock;
        this->falseBlock = falseBlock;
    }
    void execute();
    StatementKind getKind() { return IF_STATEMENT; }

    Expr *cond;
    Statement *trueBlock;
    Statement *falseBlock;
};

class WhileStatement: public Statement {
public:
    WhileStatement(Expr *cond, Statement *block){
        this->cond = cond;
        this->block = block;
    }
    void execute();
    StatementKind getKind() { return WHILE_STATEMENT; }

    Expr *cond;
    Statement *block;
};

class ForStatement: public Statement{
public:
      ForStatement(ExprList *init, Expr *cond, Statement *count, Statement *block){
          this->init = init;
          this->cond = cond;
          this->count = count;
          this->block = block;
      }

      void execute();
      StatementKind getKind() { return FOR_STATEMENT; }

      ExprList *init;
      Expr *cond;
      Statement *count;
      Statement *block;
};

class DeclarationStatement: public Statement{
public:
    DeclarationStatement(Type_v type_v, ExprList *ilist){
        this -> type_v = type_v;
        this-> ilist = ilist;
    }
    void execute();
    StatementKind getKind() { return DECLARATION_STATEMENT; }

    Type_v type_v;
    ExprList *ilist;
};

class ProgramStatement: public Statement{
public:
  ProgramStatement(string id, StatementList *header, Statement *methods) {
                              this->id = id;
                              this->header = header;
                              this->methods=methods;
                            }
  void execute();
  StatementKind getKind(){ return PROGRAM_STATEMENT; }

  string id;
  StatementList *header;
  Statement *methods;

};

class MethodStatement: public Statement{
public:
  MethodStatement(int type, string id, Statement *param, Statement *block){
                              this->type = type;
                              this->id = id;
                              this->param = param;
                              this->block = block;
                            }
  void execute();
  StatementKind getKind() { printf("%s\n", "getKind (METHOD_STATEMENT)");  return METHOD_STATEMENT; }

  int type;
  string id;
  Statement *param;
  Statement *block;
  // type 0 = int; 1 = bool; 9 = void
};

class MethodCallStatement: public Statement{
public:
    MethodCallStatement(string id, ExprList *param){
        this->id=id;
        this->param;
    }

    void execute();
    StatementKind getKind() { return METHODCALL_STATEMENT; }

    string id;
    ExprList *param;
};

class ReadStatement: public Statement{
public:
    ReadStatement(Expr *e){ this -> e = e;}
    void execute();
    StatementKind getKind() { return READ_STATEMENT; }

    Expr *e;
};

class ReturnStatement: public Statement{
public:
    ReturnStatement(Expr *e) { this->e = e; }
    void execute();
    StatementKind getKind() { return RETURN_STATEMENT; }

    Expr *e;
};

#endif
