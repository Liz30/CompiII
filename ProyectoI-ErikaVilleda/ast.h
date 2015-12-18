#ifndef _AST_H_
#define _AST_H_

#include <string>
#include <list>
#include <iostream>
#include <map>

using namespace std;

enum Type_v { INT, BOOL, VOID, _NULL };
enum Type_e { SINTAX, SEMANTIC };

extern map<string, Type_v> vars_type; // id, tipo
extern map<string, int> size_arrays; // id, size. el tipo esta en vars_type, este es solo para saber el size
extern map<string, int> vars_value;  // id, valor
extern map<string, int*> arrays_value; // id, int[];
extern bool v_main;
extern int line;

class Expr;
class Statement;
typedef list<Expr*> ExprList;
typedef list<Statement*> StatementList;

enum ExpressionKind
{
  EXPRESSION,
  BINARY_EXPRESSION,
  LESSTHAN_EXPRESSION,
  GREATERTHAN_EXPRESSION,
  LESSTHANEQUAL_EXPRESSION,
  GREATERTHANEQUAL_EXPRESSION,
  EQUAL_EXPRESSION,
  NOTEQUAL_EXPRESSION,
  OR_EXPRESSION,
  AND_EXPRESSION,
  ADD_EXPRESSION,
  SUB_EXPRESSION,
  MULT_EXPRESSION,
  DIV_EXPRESSION,
  MOD_EXPRESSION,
  ID_EXPRESSION,
  NUM_EXPRESSION,
  STRING_EXPRESSION,
  BOOL_EXPRESSION,
  ARRAY_EXPRESSION,
  FUNCTION_EXPRESSION,
  ERROR_EXPRESSION,
  WARNING_EXPRESSION
};


class Expr {
public:
    virtual int evaluate() = 0;
    virtual ExpressionKind getKind() = 0;
};

class BinaryExpr: public Expr {
public:
    BinaryExpr(Expr *expr1, Expr *expr2) {
        this->expr1 = expr1;
        this->expr2 = expr2;
    }
    ExpressionKind getKind() { return BINARY_EXPRESSION; }

    Expr *expr1;
    Expr *expr2;
};

class LTExpr: public BinaryExpr {
public:
    LTExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    ExpressionKind getKind() { return LESSTHAN_EXPRESSION; }

    int evaluate() { return expr1->evaluate() < expr2->evaluate(); }
};

class GTExpr: public BinaryExpr {
public:
    GTExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() > expr2->evaluate(); }
    ExpressionKind getKind() { return GREATERTHAN_EXPRESSION; }
};

class LTEExpr: public BinaryExpr {
public:
    LTEExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() <= expr2->evaluate(); }
    ExpressionKind getKind() { return LESSTHANEQUAL_EXPRESSION; }
};

class GTEExpr: public BinaryExpr {
public:
    GTEExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() >= expr2->evaluate(); }
    ExpressionKind getKind() { return GREATERTHANEQUAL_EXPRESSION;}
};

class NEExpr: public BinaryExpr {
public:
    NEExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() != expr2->evaluate(); }
    ExpressionKind getKind() { return NOTEQUAL_EXPRESSION; }
};

class EQExpr: public BinaryExpr {
public:
    EQExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() == expr2->evaluate(); }
    ExpressionKind getKind() {return EQUAL_EXPRESSION; }
};

class ORExpr: public BinaryExpr{
public:
    ORExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() || expr2->evaluate(); }
    ExpressionKind getKind() {return OR_EXPRESSION; }
};

class ANDExpr: public BinaryExpr{
public:
    ANDExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() && expr2->evaluate(); }
    ExpressionKind getKind() {return AND_EXPRESSION; }
};

class AddExpr: public BinaryExpr {
public:
    AddExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() + expr2->evaluate(); }
    ExpressionKind getKind() {return ADD_EXPRESSION;}
};

class SubExpr: public BinaryExpr {
public:
    SubExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() - expr2->evaluate(); }
    ExpressionKind getKind() { return SUB_EXPRESSION;}
};

class MultExpr: public BinaryExpr {
public:
    MultExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() * expr2->evaluate(); }
    ExpressionKind getKind() {return MULT_EXPRESSION;}
};

class DivExpr: public BinaryExpr {
public:
    DivExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() / expr2->evaluate(); }
    ExpressionKind getKind() {return DIV_EXPRESSION;}
};

class ModExpr: public BinaryExpr{
public:
    ModExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() % expr2 -> evaluate(); }
    ExpressionKind getKind() {return MOD_EXPRESSION;}
};

class NumExpr: public Expr {
public:
    NumExpr(int value) { this->value = value; }
    int evaluate() { return value; }
    ExpressionKind getKind() {return NUM_EXPRESSION;}

    int value;
};

class IdExpr: public Expr {
public:
    IdExpr(Type_v t, string id) { this->t = t; this->id = id; }
    int evaluate() { return vars_value[id]; }
    ExpressionKind getKind() {return ID_EXPRESSION;}

    Type_v t;
    string id;
};

class StringExpr: public Expr {
public:
    StringExpr(string &str) { this->str = str; }
    int evaluate() { return -1; }
    ExpressionKind getKind() {return STRING_EXPRESSION;}
    string str;
};

class BoolExpr: public Expr{
public:
    BoolExpr(string value) { this->value = value; }
    int evaluate() { if (value=="true") return 1; else return 0;}
    ExpressionKind getKind() { return BOOL_EXPRESSION;}
    string value;
};

class ArrayExpr: public Expr{
public:
    ArrayExpr(string id, int size){
                                    this->id=id;
                                    this->size = size;
                                  }
    int evaluate(){ return arrays_value[id][size]; };
    ExpressionKind getKind(){return ARRAY_EXPRESSION; }
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
  ExpressionKind getKind(){return FUNCTION_EXPRESSION;}

  string id;
  ExprList *param;
};

class ErrorExpr: public Expr{
public:
    ErrorExpr(Type_e t, string msg, int l){
        this->t = t;
        this->msg = msg;
        this->l = l;
    }

    int evaluate() { return 0;}
    ExpressionKind getKind() { return ERROR_EXPRESSION;}
    void show(){
        cout<<getTxt()<<" Error. "<<msg<<"\n";//Line: "<<l<<"\n";
    }

    string getTxt()
    {
        if (t==0)
          return "SINTAX";
        return "SEMANTIC";
    }

    Type_e t;
    string msg;
    int l;
};

class WarningExpr: public Expr{
public:
    WarningExpr(string msg, int l){
        this->msg = msg;
        this->l = l;
    }

    int evaluate() { return 0;}
    ExpressionKind getKind() { return WARNING_EXPRESSION;}
    void show(){
        cout<<" Warning. "<<msg<<"\n";//Line: "<<l<<"\n";
    }

    string msg;
    int l;
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
    RETURN_STATEMENT,
    BREAK_STATEMENT,
    CONTINUE_STATEMENT
};

class Statement {
public:
    virtual void execute() = 0;
    virtual StatementKind getKind() = 0;
};

class BlockStatement: public Statement {
public:
    BlockStatement(StatementList *declaration, StatementList *stList) {
                    this->stList = stList;
                    this->declaration = declaration; }
    void execute();
    StatementKind getKind() { return BLOCK_STATEMENT; }

    StatementList *declaration;
    StatementList *stList;
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

    Type_v type_v;  // si es null ya existe
    string id;
    Expr *expr;
};

class AssignStatementArray: public Statement {
public:
    AssignStatementArray(Type_v type_v, string id, Expr *pos, Expr *expr){
        this->id = id;
        this->pos = pos;
        this->expr = expr;
        this->type_v = type_v;
    }
    void execute();
    StatementKind getKind() { return ASSIGN_STATEMENT; }

    string id;
    Expr *expr;
    Expr *pos;
    Type_v type_v;
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
      ForStatement(ExprList *init, Expr *cond, StatementList *count, Statement *block){
          this->init = init;
          this->cond = cond;
          this->count = count;
          this->block = block;
      }

      void execute();
      StatementKind getKind() { return FOR_STATEMENT; }
      /// Validar que init no venga en 0
      ExprList *init;
      Expr *cond;
      StatementList *count;
      Statement *block;
};

class DeclarationStatement: public Statement{
public:
    DeclarationStatement(Type_v type_v, ExprList *ilist){
        this -> type_v = type_v;
        this-> ilist = ilist; //validar que sean expression id
    }
    void execute();
    StatementKind getKind() { return DECLARATION_STATEMENT; }
    //void inMethod(string id, ExprList *list);

    Type_v type_v;
    ExprList *ilist;
};

class ProgramStatement: public Statement{
public:
  ProgramStatement(string id, StatementList *header, StatementList *methods) {
                              this->id = id;
                              this->header = header;
                              this->methods=methods;
                            }
  void execute();
  StatementKind getKind(){ return PROGRAM_STATEMENT; }

  string id;
  StatementList *header;
  StatementList *methods;

};

class MethodStatement: public Statement{
public:
  MethodStatement(Type_v type, string id, ExprList *param, Statement *block){
                              this->type = type;
                              this->id = id;
                              this->param = param;
                              this->block = (BlockStatement*)block;
                            }
  void execute();
  StatementKind getKind() { printf("%s\n", "getKind (METHOD_STATEMENT)");  return METHOD_STATEMENT; }
  void SetParamenters(ExprList *eL);
  void inMethod(Type_v type, ExprList *eL);
  bool existVarTemp(string idVar);

  Type_v type;
  string id;
  ExprList *param;
  BlockStatement *block;
  /*map<string, Type_v> vars_type_temp;
  map<string, int> size_arrays_temp; // el tipo esta en vars_type, este es solo para saber el size
  map<string, int> vars_value_temp; //contiene el valor de cada id, el resultado de evaluate()
  map<string, int*> arrays_value_temp; // id, int[]. Valores de los arreglos*/
    // si es void, tener un flag o buscar en la lista de methodos si ya existe el void MAIN
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
    ReadStatement(ExprList *e){ this -> e = e;}
    void execute();
    StatementKind getKind() { return READ_STATEMENT; }

    ExprList *e;
};

class ReturnStatement: public Statement{
public:
    ReturnStatement(Expr *e) { this->e = e; }
    void execute();
    StatementKind getKind() { return RETURN_STATEMENT; }

    Expr *e;
};

class BreakStatement: public Statement{
public:
    BreakStatement() {};//int flag) { this->flag = flag; }
    void execute();
    StatementKind getKind() { return BREAK_STATEMENT; }

    //int flag; // 1 true; 0 false;
};

class ContinueStatement: public Statement{
public:
    ContinueStatement() {};
    void execute();
    StatementKind getKind() { return CONTINUE_STATEMENT; }
};

#endif
