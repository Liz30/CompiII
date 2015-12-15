#include <cstdio>
#include "ast.h"

// extern enum VarType {INTEGER, BOOL};
map<string, int> nmethods;
map<string, int> vars;

struct varsT{
  string id;
  int value;
  int type; // 0 Int; 1 Bool
  int method; // 9 Main
};

map<string, int> vars_type; // 0 Int; 1 Bool

void BlockStatement::execute()
{
    printf("BlockStatement\n");
    /*list<Statement *>::iterator it = stList.begin();

    while (it != stList.end()) {
        Statement *st = *it;

        st->execute();
        it++;
        printf("%s\n", st->getKind());
    }*/
}

void PrintStatement::execute()
{
    printf("Print Beginning\n");
    ExprList::iterator it = expr->begin();
    int result;

    while (it != expr->end()) {
      Expr *ex = *it;

      result=ex->evaluate();
      printf("%d\n", result);
      it++;
    }
    printf("\n");
}

void AssignStatement::execute()
{
    printf("AssignStatement\n");
    //int result = expr->evaluate();
    //vars[id] = result;
}

void AssignStatementArray::execute()
{
  printf("AssignStatementArray\n");
}

void IfStatement::execute()
{
    printf("IfStatement\n");
    int result = cond->evaluate();

    if (result) {
        trueBlock->execute();
    } else if (falseBlock != 0) {
        falseBlock->execute();
    }
}

void WhileStatement::execute()
{
  printf("WhileStatement\n");
  while (cond->evaluate()) {
    block->execute();
  }
}

void ForStatement::execute()
{
  printf("ForStatement\n");
  /*for ( init->execute(); cond->evaluate(); count->execute()) {
    block->execute();
  }*/
}

void DeclarationStatement::execute()
{
  printf("DeclarationStatement\n");
}

void ProgramStatement::execute()
{
  printf("ProgramStatement\n");
  /*Statement *vars;
  Statement *mvoid;
  Statement *methods;*/

  /*Statement::iterator it = mvoid.begin();
  while (it != mvoid.end()) {
    Statement *st = *it;

    st->execute();
    it++;
  }*/

  //methods->execute();
  //methods->execute();
}

void MethodStatement::execute()
{
  printf("MethodStatement\n");
}

void MethodCallStatement::execute()
{
  printf("MethodCallStatement\n");
}

void ReadStatement::execute()
{
  printf("ReadStatement\n");
}

void ReturnStatement::execute()
{
  printf("ReturnStatement\n");
}

void BreakStatement::execute()
{
  printf("BreakStatement\n");
}

void ContinueStatement::execute()
{
  printf("ContinueStatement\n");
}
